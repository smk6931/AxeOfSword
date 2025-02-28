// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAnim.h"
#include "TimerManager.h"
#include "AxeOfSword/Mk_Boss/BossMk/Boss/BossMk.h"
#include "AxeOfSword/Mk_Boss/BossMk/Sword/Sword.h"
#include "Components/CapsuleComponent.h"

// tick 실시간으로 계속 돌아가는 이벤트
void UBossAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (BossMk)
	{
		animState = BossMk->Fsm->mState;
	}
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
	
	UE_LOG(LogTemp, Warning, TEXT("Dash And Damage 10?"));
	JumpDir = FMath::RandRange(-90, 90);
	FRotator Rotation = FRotator(0.0f, JumpDir, 0.0f);

	FVector Direction = Rotation.RotateVector(BossMk->GetActorForwardVector());
	// Direction.Z = 5.0f;
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
