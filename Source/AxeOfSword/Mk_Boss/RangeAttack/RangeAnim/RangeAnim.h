// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AxeOfSword/Mk_Boss/RangeAttack/RangeFSM/RangeFSM.h"
#include "RangeAnim.generated.h"

/**
 * 
 */
UCLASS()
class AXEOFSWORD_API URangeAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeBeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Animation")
	void PlayJumpAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category="Animation")
	void PlayStuckDamage();

	UFUNCTION(BlueprintImplementableEvent, Category="Animation")
	void PlayJumpBackFlip();

	UFUNCTION(BlueprintImplementableEvent, Category="Animation")
	void PlayDieAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	class ARangeMonster* RangeMonster;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	class URangeFSM* RangeFSM;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Animation")
	float Horizontal;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Animation")
	float Vertical;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Animation")
	ERangeFSMState animState;
};




