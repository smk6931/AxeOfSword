// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnim.h"

#include "AxeOfSword/Mk_Boss/Boss/BossMk.h"

void UBossAnim::AnimNotify_JumpAttackEnd()
{
	// bRgAttack = true;
	// bJumpAttack = false;
	FString stateStr = UEnum::GetValueAsString(animState);
	UE_LOG(LogTemp, Display, TEXT("JumpAttackEnd: %s"), *stateStr)
	if (animState == EEnemyState::JumpAttack)
	{
		animState = EEnemyState::RgAttack;
	}
	else
	{
		animState = EEnemyState::Move;
	}
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
	animState = EEnemyState::Attack;
}

void UBossAnim::AnimNotify_DamageEnd()
{
	UE_LOG(LogTemp, Display, TEXT("DamageEnd"));
	ABossMk* BossMk = Cast<ABossMk>(GetOwningActor());
	BossMk->Fsm->mState = EEnemyState::idle;
	animState = EEnemyState::idle;
}
