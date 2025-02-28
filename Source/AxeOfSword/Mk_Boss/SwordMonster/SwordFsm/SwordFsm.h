// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwordFsm.generated.h"

UENUM(BlueprintType)
enum class ESwordState : uint8
{
	idle UMETA(DisplayName = "Idle"),
	Move,
	OneAttack,
	TwiceAttack,
	ThirdAttack,
	Dash
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXEOFSWORD_API USwordFsm : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USwordFsm();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	ESwordState SwordState = ESwordState::idle;

	void Idle();
	void Move();
	void OneAttactk();
	void TwiceAttackk();
	void ThirdAttackk();
	void Dash();

	void SwitchSwordState();

	FTimerHandle SwordTimer;
};
