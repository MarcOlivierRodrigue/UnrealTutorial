// Copyright Marc-Olivier Rodrigue 2021


#include "OpenDoor.h"
#include "GameFramework/Actor.h"

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

	StartingYaw = GetOwner() -> GetActorRotation().Yaw;
	CurrentYaw = StartingYaw;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, StartingYaw + RotationYaw,  DeltaTime, 110);
	GetOwner() -> SetActorRotation(FRotator(0.f, CurrentYaw, 0.f));
}

