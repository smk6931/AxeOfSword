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
	JumpAttack,
	RgAttack,
	TrippleAttack,
	Dash,
	Damage,
	Die
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
	float AttackTime = 2;
	float IdleDelayTime = 2;
	float CurrentTime = 0;
	float AttackRange = 150;
	
	void IdleState();
	void MoveState();
	
	void JumpAttack();
	void RgAttack();
	void TrippleAttack();
	void Dash();
	
	void AttackState();
	void DamageState();
	void DieState();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class ABossMk* Boss;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = FSM)
	class APlayerCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = FSM)
	class UBossAnim* Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = FSM)
	class ASword* Sword;
};
