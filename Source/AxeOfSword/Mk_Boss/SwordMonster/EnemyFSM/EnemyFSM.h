// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	idle UMETA(DisplayName = "Idlestate"),
	Move,
	Attack,
	RgAttack,
	TrippleAttack,
	Dash,
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXEOFSWORD_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	EEnemyState mState = EEnemyState::idle;
	
	UPROPERTY(EditAnywhere, Category=FSM)
	float AttackTime = 1.2f;
	float IdleDelayTime = 1.5;
	float CurrentTime = 0;
	float AttackRange = 250.0f;
	
	void IdleState();
	void MoveState();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool Walk = true;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool AttackStay = true;
	
	void AttackState();
	void RgAttack();
	void TripleAttack();
	void Dash();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class ABossMk* Boss;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = FSM)
	class APlayerCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = FSM)
	class UBossAnim* Anim;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = FSM)
	// class ASword* Sword;
};
