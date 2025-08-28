// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle == nullptr)
	{
		return;
	}

	UPrimitiveComponent* GrabbedComponent = PhysicsHandle->GetGrabbedComponent();
	if (GrabbedComponent)
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}


void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle == nullptr)
	{
		return;
	}

	UPrimitiveComponent* GrabbedComponent = PhysicsHandle->GetGrabbedComponent();
	 
	if (GrabbedComponent) 
	{
		GrabbedComponent->WakeAllRigidBodies();
		GrabbedComponent->GetOwner()->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
	}
}


void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FHitResult OutHitResult;
	bool HasHit = GetGrabbableItemInReach(OutHitResult);

	if (HasHit)
	{
		DrawDebugSphere(GetWorld(), OutHitResult.ImpactPoint, 10, 10, FColor::Cyan, false, 5);

		UPrimitiveComponent* HitComponent = OutHitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();

		AActor* Actor = OutHitResult.GetActor();
		Actor->Tags.Add("Grabbed");
		Actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandle->GrabComponentAtLocationWithRotation(HitComponent,
			NAME_None,
			OutHitResult.ImpactPoint,
			GetComponentRotation());
	}
}

bool UGrabber::GetGrabbableItemInReach(FHitResult& OutHitResult) const
{

	FVector Start = GetComponentLocation();
	FVector End = Start + (GetForwardVector() * MaxGrabDistance);

	ECollisionChannel GrabberChannel = ECC_GameTraceChannel2;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);

	return GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, GrabberChannel, Sphere);
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GrabberRequires a UPhysicsHandleComponent"));
	}

	return Result;
}
