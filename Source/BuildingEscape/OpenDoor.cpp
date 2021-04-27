// Copyright Marc-Olivier Rodrigue 2021


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	if(!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s has an OpenDoor component, but no pressure plate set."), *GetOwner() -> GetName());
	}

	StartingYaw = GetOwner() -> GetActorRotation().Yaw;
	CurrentYaw = StartingYaw;
	CalcTargetYaw();
	FindAudioComponent();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(PressurePlate && DoorMovementAudio) 
	{
		float currTotalActorsMass = CalcTotalActorsMass();
		if (currTotalActorsMass >= RequiredMassToOpen)
		{
			hasCloseAudioPlayed = false;
			DoorLastOpened = GetWorld() -> GetTimeSeconds();
			OpenDoor(DeltaTime);
		}
		else if (DoorLastOpened != 0.f && GetWorld() -> GetTimeSeconds() - DoorLastOpened >= DoorCloseDelay)
		{
			hasOpenAudioPlayed = false;
			CloseDoor(DeltaTime);
		}
	}
}


void UOpenDoor::OpenDoor(float DeltaTime)
{ 
	if (CurrentYaw != TargetYaw)
	{
		CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, TargetYaw,  DeltaTime, DoorOpenSpeed);
		GetOwner() -> SetActorRotation(FRotator(0.f, CurrentYaw, 0.f));
	}
	
	if (!hasOpenAudioPlayed) 
	{
		DoorMovementAudio -> Play();
		hasOpenAudioPlayed = true;
	}

}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	if (CurrentYaw != StartingYaw)
	{
		CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, StartingYaw,  DeltaTime, DoorCloseSpeed);
		GetOwner() -> SetActorRotation(FRotator(0.f, CurrentYaw, 0.f));
	}
	else if (!hasCloseAudioPlayed) 
	{
		DoorMovementAudio -> Play();
		hasCloseAudioPlayed = true;
	}
}

void UOpenDoor::FindAudioComponent()
{
	DoorMovementAudio = GetOwner() -> FindComponentByClass<UAudioComponent>();
	if (!DoorMovementAudio)
	{
		UE_LOG(LogTemp, Error, TEXT("AActor %s must have a UAudioComponent"), *GetOwner() -> GetName());
	}
}

const float UOpenDoor::CalcTotalActorsMass()
{
	float TotalMass = 0.f;
	TArray<UPrimitiveComponent*> OverlappingComponents;
	PressurePlate -> GetOverlappingComponents(OUT OverlappingComponents);
	
	for (UPrimitiveComponent* Component : OverlappingComponents)
	{
		if (Component) 
		{
			TotalMass += Component -> GetMass();
		}
	}

	return TotalMass;
}

void UOpenDoor::CalcTargetYaw()
{
	TargetYaw = StartingYaw + OpenAngle;
	while (TargetYaw >= 360.f)
	{
		TargetYaw -= 360.f;
	}
}