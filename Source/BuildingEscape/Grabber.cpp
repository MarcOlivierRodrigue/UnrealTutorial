// Copyright Marc-Olivier Rodrigue 2021
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
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

	UE_LOG(LogTemp, Warning, TEXT("Grabber here!"));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;

	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);

	FVector EndPoint = PlayerViewLocation + PlayerViewRotation.Vector() * Reach;

	DrawDebugLine(
		GetWorld(),
		PlayerViewLocation, 
		EndPoint,
		FColor(0, 255, 0, 1),
		false,
		0.f,
		0,
		3.f
	);
}

