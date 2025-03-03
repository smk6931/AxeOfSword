#include "BossAnim.h"
#include "TimerManager.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/Boss/BossMk.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/Sword/Sword.h"
#include "Components/CapsuleComponent.h"


void UBossAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (EnemyFsm)
	{
		animState = EnemyFsm->mState;
	}
}

void UBossAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	BossMk = Cast<ABossMk>(GetOwningActor());
	EnemyFsm = Cast<UEnemyFSM>(BossMk->Fsm);
}

void UBossAnim::AnimNotify_AaFir()
{
	BossMk->BossSword->SwordCapsule->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
}

void UBossAnim::AnimNotify_AaEnd()
{
	
	FString stateStr = UEnum::GetValueAsString(animState);
	if (EnemyFsm->mState == EEnemyState::Attack)
	{
		EnemyFsm->mState = EEnemyState::RgAttack;
	}
	else
	{
		EnemyFsm->mState = EEnemyState::Move;
	}

	BossMk->BossSword->SwordCapsule->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void UBossAnim::AnimNotify_AbEnd()
{
	EnemyFsm->mState = EEnemyState::TrippleAttack;

	BossMk->BossSword->SwordCapsule->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void UBossAnim::AnimNotify_AcEnd()
{
	EnemyFsm->mState = EEnemyState::Dash;
	BossMk->BossSword->SwordCapsule->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}
// 대쉬 초반 공격
void UBossAnim::AnimNotify_DashFir()
{
	JumpDir = FMath::RandRange(-90, 90);
	FRotator Rotation = FRotator(0.0f, JumpDir, 0.0f);

	FVector Direction = Rotation.RotateVector(BossMk->GetActorForwardVector());
	FVector Velocity = Direction * 1500;
	BossMk->LaunchCharacter(Velocity, true,true);
}

void UBossAnim::AnimNotify_DashEnd()
{
	EnemyFsm->mState = EEnemyState::idle;
}
