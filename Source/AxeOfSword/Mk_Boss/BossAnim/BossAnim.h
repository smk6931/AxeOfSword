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

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
 //    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// class ABossMk* Boss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEnemyFSM* EnemyFsm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ABossMk* BossMk;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	EEnemyState animState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	bool bJumpAttack = false;

	void SwordCollisiontrue();
	void SwordCollisionfalse();
	
	UFUNCTION()
	void AnimNotify_AaFir();
	
	UFUNCTION()
	void AnimNotify_AaEnd();

	UFUNCTION()
	void AnimNotify_AbEnd();

	UFUNCTION()
	void AnimNotify_AcEnd();

	UFUNCTION()
	void AnimNotify_DashFir();
	
	UFUNCTION()
	void AnimNotify_DashEnd();

	FTimerHandle TimerHandle;
};
