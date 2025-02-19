// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnim.h"

#include "AxeOfSword/Mk_Boss/Boss/BossMk.h"
#include "AxeOfSword/Mk_Boss/Sword/Sword.h"
#include "Components/CapsuleComponent.h"

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

	ABossMk* BossMk = Cast<ABossMk>(GetOwningActor());
	if (BossMk)
	{
		//보스Mk의 저장된 보스 스워드 - 캡슐 콜리전을 비활성화 하고 싶다
		BossMk->BossSword->SwordCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

void UBossAnim::AnimNotify_OneAttack()
{
	//보스 헤더파일 담아둔 Boss Sword를 가지고 온다
	ABossMk* BossMk = Cast<ABossMk>(GetOwningActor());
	BossMk->BossSword->SwordCapsule->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
}