// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAnim.h"
#include "TimerManager.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/Boss/BossMk.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/Sword/Sword.h"
#include "Components/CapsuleComponent.h"


void UBossAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	// EnemyFsm과 Mstate를 동기화 한다
	// animState = EnemyFsm->mState;
	// EnemyFsm->mState = animState;
}

void UBossAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

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
	JumpDir = FMath::RandRange(-90, 90);
	FRotator Rotation = FRotator(0.0f, JumpDir, 0.0f);

	FVector Direction = Rotation.RotateVector(BossMk->GetActorForwardVector());
	// FVector Right = BossMk->GetActorForwardVector();
	FVector Velocity = Direction * 750;
	BossMk->LaunchCharacter(Velocity, true,true);
}

void UBossAnim::AnimNotify_DashEnd()
{
	animState = EEnemyState::idle;
	BossMk->Fsm->mState = animState;
}

void UBossAnim::PrintViewPort()
{
	if (GEngine)
	{
		FString StateMessage = FString::Printf(TEXT("Current Animation State: %s"),animState);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, StateMessage);
	}
}
