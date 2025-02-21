// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAnim.h"
#include "TimerManager.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/Boss/BossMk.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/Sword/Sword.h"
#include "Components/CapsuleComponent.h"


void UBossAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	BossMk = Cast<ABossMk>(GetOwningActor());
}

void UBossAnim::AnimNotify_AaFir()
{
	BossMk->BossSword->SwordCapsule->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
}

void UBossAnim::AnimNotify_AaEnd()
{
	FString stateStr = UEnum::GetValueAsString(animState);

	if (animState == EEnemyState::JumpAttack)
	{
		animState = EEnemyState::RgAttack;
	}
	else
	{
		animState = EEnemyState::Move;
	}
	BossMk->BossSword->SwordCapsule->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void UBossAnim::AnimNotify_AbEnd()
{
	animState = EEnemyState::TrippleAttack;

	BossMk->BossSword->SwordCapsule->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void UBossAnim::AnimNotify_AcEnd()
{
	animState = EEnemyState::Dash;
	BossMk->BossSword->SwordCapsule->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}
// 대쉬 초반 공격
void UBossAnim::AnimNotify_DashFir()
{
	FVector Right = BossMk->GetActorRightVector();
	FVector Velocity = Right * 2500;
	BossMk->LaunchCharacter(Velocity, true,true);
	BossMk->BossSword->SwordCapsule->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
}

void UBossAnim::AnimNotify_DashEnd()
{
	animState = EEnemyState::Attack;
	BossMk->BossSword->SwordCapsule->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}
