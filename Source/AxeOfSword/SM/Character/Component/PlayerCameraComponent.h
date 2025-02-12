// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AxeOfSword/SM/Helper/UtilHelper.h"
#include "Camera/CameraComponent.h"
#include "PlayerCameraComponent.generated.h"

UCLASS()
class AXEOFSWORD_API UPlayerCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UPlayerCameraComponent();

	void SetCameraOption(const FVector& MoveTo, const int8 Fov = 90);

	void RollbackToFirstCameraOption();

	void EnableCombatCameraMode();
	
	GETTER(FVector, CurrentLocation)
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Option", meta = (AllowPrivateAccess = true))
	FVector CombatRelativeLocation;
	
	FVector CurrentLocation;
	
	FVector DefaultLocation;

	uint8 DefaultFov;
};