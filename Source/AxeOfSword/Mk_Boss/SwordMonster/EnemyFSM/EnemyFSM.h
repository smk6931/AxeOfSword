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
	Undefined,
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
	//공격 딜레이타임
	float AttackTime = 1.2f;
	//Idle 딜레이타임
	float IdleDelayTime = 1.5;
	float CurrentTime = 0;
	//공격범위
	float AttackRange = 250.0f;
	
	void IdleState();
	void MoveState();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool Walk = true;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool AttackStay = true;

	// 첫번쨰 / 두번째 / 세번째 / 피하기
	void AttackState();
	void RgAttack();
	void TripleAttack();
	void Dash();

	void DrawDebugCircleAroundPlayer();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class ABossMk* Boss;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = FSM)
	class APlayerCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = FSM)
	class UBossAnim* Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = FSM)
	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = FSM)
	float AttackDistance = 2000.0f;
};
