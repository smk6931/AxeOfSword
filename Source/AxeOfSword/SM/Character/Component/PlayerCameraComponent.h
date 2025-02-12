// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "PlayerCameraComponent.generated.h"

UCLASS()
class AXEOFSWORD_API UPlayerCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UPlayerCameraComponent();

	void AddCameraOption(const FVector& MoveTo, const int8 Fov);

	void RollbackToFirstCameraOption();
	
protected:
	virtual void BeginPlay() override;
	
private:
	FVector DefaultLocation;

	uint8 DefaultFov;
};
