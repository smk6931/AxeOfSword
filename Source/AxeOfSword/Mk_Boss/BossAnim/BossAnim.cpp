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

void UBossAnim::SwordCollisiontrue()
{
	BossMk->BossSword->SwordCapsule->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
}
void UBossAnim::SwordCollisionfalse()
{
	BossMk->BossSword->SwordCapsule->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void UBossAnim::AnimNotify_AaFir()
{
	if (BossMk->BossSword)
	{
		if (BossMk)
		{
			SwordCollisiontrue();
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBossAnim::SwordCollisionfalse, 1.0f, false);
			// GetWorldTimerManager().SetTimer(TimerHandle, this, &ABossMk::DestroyBossSword, 1.0f, false);
		}
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
	
	if (BossMk->BossSword)
	{
		if (BossMk)
		{
			SwordCollisionfalse();
		}
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
	SwordCollisionfalse();
}
