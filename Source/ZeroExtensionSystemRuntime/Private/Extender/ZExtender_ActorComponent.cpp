// Copyright Zero Games. All Rights Reserved.

#include "Extender/ZExtender_ActorComponent.h"

#include "ZeroExtensionSystemRuntimeLogChannels.h"

namespace ZES::ZExtender_ActorComponent_Private
{
	static void ApplyPostProcessors(UActorComponent* component, const FZExtender_ActorComponentRequest& request, EZExtender_ActorComponentPostProcessorRunningGroup runningGroup)
	{
		for (const auto& postProcessor : request.PostProcessors)
		{
			if (postProcessor->GetRunningGroup() == runningGroup)
			{
				postProcessor->Apply(component);
			}
		}
	}
}

bool UZExtender_ActorComponent::Extend(UObject* extendee)
{
	auto actor = CastChecked<AActor>(extendee);
	TArray<TTuple<TWeakObjectPtr<UActorComponent>, const FZExtender_ActorComponentRequest*>> pendingPostProcessors;

	// IMPORTANT: Assume request array is not changed in this function.
	for (const auto& request : Requests)
	{
		// Assume component constructor doesn't kill the actor.
		ApplyRequest(actor, request, pendingPostProcessors);
	}

	// Initialize/Register after all components of this extender (treat as a group) has been created so components can set up dependencies with each other.
	for (const auto& pendingPostProcessor : pendingPostProcessors)
	{
		if (UActorComponent* component = pendingPostProcessor.Get<0>().Get())
		{
			// Always register regardless of bAutoRegister.
			component->RegisterComponent();

			// RegisterComponent() can kill the actor.
			if (actor->IsActorBeingDestroyed() || actor->HasAllFlags(RF_MirroredGarbage))
			{
				return false;
			}

			// Assume post processor doesn't kill the actor.
			ZES::ZExtender_ActorComponent_Private::ApplyPostProcessors(component, *pendingPostProcessor.Get<1>(), EZExtender_ActorComponentPostProcessorRunningGroup::PostRegisterToWorld);
		}
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

void UZExtender_ActorComponent::ApplyRequest(AActor* extendee, const FZExtender_ActorComponentRequest& request, TArray<TTuple<TWeakObjectPtr<UActorComponent>, const FZExtender_ActorComponentRequest*>>& pendingPostProcessors)
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

	FStaticConstructObjectParameters params { request.ComponentClass };
	params.Outer = extendee;
	params.Name = request.ComponentName;
	params.SetFlags = RF_Transient;
	params.PropertyInitCallback = [&request]
	{
		auto component = CastChecked<UActorComponent>(FUObjectThreadContext::Get().TopInitializerChecked().GetObj());
		ZES::ZExtender_ActorComponent_Private::ApplyPostProcessors(component, request, EZExtender_ActorComponentPostProcessorRunningGroup::PostInitProperties);
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

	ZES::ZExtender_ActorComponent_Private::ApplyPostProcessors(component, request, EZExtender_ActorComponentPostProcessorRunningGroup::PreRegisterToWorld);

	pendingPostProcessors.Emplace(TTuple<TWeakObjectPtr<UActorComponent>, const FZExtender_ActorComponentRequest*> { component, &request });
}


