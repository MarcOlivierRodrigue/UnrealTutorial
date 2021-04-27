// Copyright Marc-Olivier Rodrigue 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	FRotator DoorRotation;
	float StartingYaw; 
	float CurrentYaw;
	float DoorLastOpened = 0.f;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	const float CalcTotalActorsMass();

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.f;

	UPROPERTY(EditAnywhere)
	float RequiredMassToOpen = 50.f;

	UPROPERTY(EditAnywhere)
	int32 DoorOpenSpeed = 110;

	UPROPERTY(EditAnywhere)
	int32 DoorCloseSpeed = 110;
};
