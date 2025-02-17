// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnim.h"

void UBossAnim::AnimNotify_JumpAttackEnd()
{
	bRgAttack = true;
	bJumpAttack = false;
}

void UBossAnim::AnimNotify_RgAttackEnd()
{
	bTrippleAttack = true;
	bRgAttack = false;
}

void UBossAnim::AnimNotify_TrippleAttackEnd()
{
	bDash = true;
	bTrippleAttack = false;
}

void UBossAnim::AnimNotify_DashEnd()
{
	bJumpAttack = true;
	bDash = false;
}
