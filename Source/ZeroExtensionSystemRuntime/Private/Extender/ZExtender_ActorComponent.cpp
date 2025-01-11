// Copyright Zero Games. All Rights Reserved.

#include "Extender/ZExtender_ActorComponent.h"

#include "ZeroExtensionSystemRuntimeLogChannels.h"
#include "Extender/ZExtender_ActorComponentPostProcessorBaseInterface.h"

bool UZExtender_ActorComponent::Extend(UObject* extendee)
{
	auto actor = CastChecked<AActor>(extendee);
	for (const auto& request : Requests)
	{
		ApplyRequest(actor, request);
	}

	return true;
}

bool UZExtender_ActorComponent::Revert(UObject* extendee, bool destroying)
{
	auto actor = CastChecked<AActor>(extendee);
	ON_SCOPE_EXIT { Registry.Remove(actor); };

	if (destroying)
	{
		return true;
	}
	
	TArray<TWeakObjectPtr<UActorComponent>>* registeredComponents = Registry.Find(actor);
	if (!registeredComponents)
	{
		return true;
	}

	for (const auto& weakComponent : *registeredComponents)
	{
		UActorComponent* component = weakComponent.Get();
		if (!component || component->IsBeingDestroyed() || component->HasAllFlags(RF_MirroredGarbage) || component->IsUnreachable())
		{
			// Component is already dead, skip.
			continue;
		}

		if (component->GetOwner() != actor)
		{
			// Component is renamed to other owner, skip.
			continue;
		}

		component->DestroyComponent();
	}
	
	return true;
}

bool UZExtender_ActorComponent::InternalCanExtend(UObject* extendee) const
{
	return extendee->IsA<AActor>();
}

void UZExtender_ActorComponent::ApplyRequest(AActor* extendee, const FZExtender_ActorComponentRequest& request)
{
	bool shouldCreate = true;
	switch (request.CreationCondition)
	{
	case EZExtender_ActorComponentRequestCreationCondition::SkipIfExists:
		{
			shouldCreate = !extendee->FindComponentByClass(request.ComponentClass);
			break;
		}
	case EZExtender_ActorComponentRequestCreationCondition::SkipIfExistsExactClass:
		{
			for (const auto& component : extendee->GetComponents())
			{
				if (component->GetClass() == request.ComponentClass)
				{
					shouldCreate = false;
					break;
				}
			}
			break;
		}
	default:
		{
			break;
		}
	}

	if (!shouldCreate)
	{
		return;
	}

	if (!request.ComponentName.IsNone())
	{
		if (UObject* existingSubobject = StaticFindObjectFast(nullptr, extendee, request.ComponentName))
		{
			UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[UZExtender_ActorComponent::ApplyRequest] Subobject [%s] is already exists!"), *existingSubobject->GetPathName());
			return;
		}
	}

	auto applyPostProcessors = [&request](UActorComponent* component, EZExtender_ActorComponentPostProcessorRunningGroup runningGroup)
	{
		for (const auto& postProcessor : request.PostProcessors)
		{
			if (postProcessor->GetRunningGroup() == runningGroup)
			{
				postProcessor->Apply(component);
			}
		}
	};
	
	FStaticConstructObjectParameters params { request.ComponentClass };
	params.Outer = extendee;
	params.Name = request.ComponentName;
	params.SetFlags = RF_Transient;
	params.PropertyInitCallback = [&applyPostProcessors]
	{
		auto component = CastChecked<UActorComponent>(FUObjectThreadContext::Get().TopInitializerChecked().GetObj());
		applyPostProcessors(component, EZExtender_ActorComponentPostProcessorRunningGroup::PostInitProperties);
	};
	
	auto component = CastChecked<UActorComponent>(StaticConstructObject_Internal(params));
	TArray<TWeakObjectPtr<UActorComponent>>& registeredComponents = Registry.FindOrAdd(extendee);
	registeredComponents.Emplace(component);

	extendee->AddInstanceComponent(component);
	if (auto sceneComponent = Cast<USceneComponent>(component))
	{
		if (USceneComponent* existingRoot = extendee->GetRootComponent())
		{
			sceneComponent->SetupAttachment(existingRoot);
		}
		else
		{
			extendee->SetRootComponent(sceneComponent);
		}
	}

	applyPostProcessors(component, EZExtender_ActorComponentPostProcessorRunningGroup::PreRegisterToWorld);

	if (component->bAutoRegister)
	{
		component->RegisterComponent();
	}

	applyPostProcessors(component, EZExtender_ActorComponentPostProcessorRunningGroup::PostRegisterToWorld);
}


