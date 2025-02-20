// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAnim.h"
#include "TimerManager.h"
#include "AxeOfSword/Mk_Boss/Boss/BossMk.h"
#include "AxeOfSword/Mk_Boss/Sword/Sword.h"
#include "Components/CapsuleComponent.h"


void UBossAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	BossMk = Cast<ABossMk>(GetOwningActor());
}

void UBossAnim::ToggleSwordCollision(bool IsEnabled)
{
	if (IsEnabled)
	{
		BossMk->BossSword->SwordCapsule->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	}
	else
	{
		BossMk->BossSword->SwordCapsule->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	}
}

void UBossAnim::AnimNotify_AaFir()
{
	if (BossMk->BossSword)
	{
		if (BossMk)
		{
			ToggleSwordCollision(true);
		}
		// GetWorld()->GetTimerManager(TimerHandleA, this, &UBossAnim::ToggleSwordCollision(false), 0.5f, false);
	}
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
}

void UBossAnim::AnimNotify_AbEnd()
{
	animState = EEnemyState::TrippleAttack;
}

void UBossAnim::AnimNotify_AcEnd()
{
	animState = EEnemyState::Dash;
}
// 대쉬 초반 공격
void UBossAnim::AnimNotify_DashFir()
{
	FVector Right = BossMk->GetActorRightVector();
	FVector Velocity = Right * 2500;
	BossMk->LaunchCharacter(Velocity, true,true);
}

void UBossAnim::AnimNotify_DashEnd()
{
	animState = EEnemyState::Attack;
}
