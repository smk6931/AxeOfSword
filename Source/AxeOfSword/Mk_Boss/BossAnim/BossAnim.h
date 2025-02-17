// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AxeOfSword/Mk_Boss/EnemyFSM/EnemyFSM.h"
#include "BossAnim.generated.h"

/**
 * 
 */
UCLASS()
class AXEOFSWORD_API UBossAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	EEnemyState animState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	bool bJumpAttack = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	bool bRgAttack = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	bool bTrippleAttack = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	bool bDash = false;
	
	
	UFUNCTION()
	void AnimNotify_JumpAttackEnd();
	UFUNCTION()
	void AnimNotify_RgAttackEnd();
	UFUNCTION()
	void AnimNotify_TrippleAttackEnd();
	UFUNCTION()
	void AnimNotify_DashEnd();
};
