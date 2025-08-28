// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* AcceptableActor = GetAcceptableActor();

	if (AcceptableActor)
	{
		UE_LOG(LogTemp, Display, TEXT("FoundActor %s"), *AcceptableActor->GetName());

		if (Mover)
		{
			UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(AcceptableActor->GetRootComponent());

			if (Component)
			{
				Component->SetSimulatePhysics(false);
			}
			AcceptableActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
			Mover->SetShouldMove(true);
		}
	}
	else
	{
		if (Mover)
		{
			Mover->SetShouldMove(false);
		}
	}
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
	UE_LOG(LogTemp, Display, TEXT("SetMover %s"), *Mover->GetName());
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> Actors;

	GetOverlappingActors(Actors);

	for (AActor* Actor : Actors)
	{
		if (Actor->ActorHasTag(UnlockTag) && !Actor->ActorHasTag("Grabbed"))
		{
			return Actor;
		}
	}

	return nullptr;
}
