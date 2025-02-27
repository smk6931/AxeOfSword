// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeAnim.h"

#include "AxeOfSword/Mk_Boss/RangeAttack/RangeMonster/RangeMonster.h"

void URangeAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	RangeMonster = Cast<ARangeMonster>(TryGetPawnOwner());

	RangeFSM = Cast<URangeFSM>(TryGetPawnOwner());
}

void URangeAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//범위공격 몬스터 캐스팅
}




