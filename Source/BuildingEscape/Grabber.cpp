// Copyright Marc-Olivier Rodrigue 2021
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT

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
	
	FindPhysicsHandle();
	BindInputs();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle && PhysicsHandle -> GrabbedComponent)
	{
		CalcPlayerViewReachEnd();
		PhysicsHandle -> SetTargetLocation(PlayeViewReachEnd);
	}
}

void UGrabber::Grab()
{
	FHitResult Hit = GetHitResult();
	UPrimitiveComponent* ComponentHit = Hit.GetComponent();
	if (ComponentHit) 
	{
		PhysicsHandle -> GrabComponentAtLocation(ComponentHit, NAME_None, PlayeViewReachEnd);	
	}
}

void UGrabber::Release()
{
	if (PhysicsHandle)
	{
		PhysicsHandle -> ReleaseComponent();
	}
}

void UGrabber::FindPhysicsHandle() 
{
	PhysicsHandle = GetOwner() -> FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) 
	{
		UE_LOG(LogTemp, Error, TEXT("AActor %s must have a UPhysicsHandleComponent"), *GetOwner() -> GetName());
	}
}

void UGrabber::BindInputs()
{
	//GrabHanble = GetOwner() -> FindComponentByClass<UInputComponent>();
	GrabHanble = GetOwner() -> InputComponent;
	if (GrabHanble) 
	{
		GrabHanble -> BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		GrabHanble -> BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No UInputComponent found for %s"), *GetOwner() -> GetName());
	}
}

FHitResult UGrabber::GetHitResult()
{
	CalcPlayerViewReachEnd();
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	
	GetWorld() -> LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewLocation,
		PlayeViewReachEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	
	return Hit;
}

void UGrabber::CalcPlayerViewReachEnd()
{
	FRotator PlayerViewRotation;
	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(OUT PlayerViewLocation,  OUT PlayerViewRotation);

	PlayeViewReachEnd = PlayerViewLocation + PlayerViewRotation.Vector() * Reach;
}