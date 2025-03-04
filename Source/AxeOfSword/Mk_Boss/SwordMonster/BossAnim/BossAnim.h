// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/EnemyFSM/EnemyFSM.h"
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
	virtual void NativeBeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	float JumpDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEnemyFSM* EnemyFsm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ABossMk* BossMk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ASword* Sword;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	EEnemyState animState;
	
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

	// UFUNCTION()
	// void AnimNotify_

	FTimerHandle TimerHandleA;
};
