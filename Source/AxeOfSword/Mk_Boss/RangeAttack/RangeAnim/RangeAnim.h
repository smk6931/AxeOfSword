// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RangeAnim.generated.h"

/**
 * 
 */
UCLASS()
class AXEOFSWORD_API URangeAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category="Animation")
	void PlayJumpAnimaition();
};


