// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnim.h"

void UBossAnim::AnimNotify_JumpAttackEnd()
{
	// bRgAttack = true;
	// bJumpAttack = false;
	animState = EEnemyState::RgAttack;
}

void UBossAnim::AnimNotify_RgAttackEnd()
{
	// bTrippleAttack = true;
	// bRgAttack = false;
	animState = EEnemyState::TrippleAttack;
}

void UBossAnim::AnimNotify_TrippleAttackEnd()
{
	// bDash = true;
	// bTrippleAttack = false;
	animState = EEnemyState::Dash;
}

void UBossAnim::AnimNotify_DashEnd()
{
	// bJumpAttack = true;
	// bDash = false;
	animState = EEnemyState::JumpAttack;
}
