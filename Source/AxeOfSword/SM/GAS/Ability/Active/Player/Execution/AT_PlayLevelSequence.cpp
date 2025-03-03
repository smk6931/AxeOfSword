#include "AT_PlayLevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/Boss/BossMk.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"

UAT_PlayLevelSequence* UAT_PlayLevelSequence::InitialEvent(UGameplayAbility* Ability, ULevelSequence* LevelSequence)
{
	UAT_PlayLevelSequence* NewTask = NewAbilityTask<UAT_PlayLevelSequence>(Ability);
	NewTask->LevelSequence = LevelSequence;
	
	return NewTask;
}

void UAT_PlayLevelSequence::Activate()
{
	Super::Activate();

	const APlayerCharacter* BaseCharacter = Cast<APlayerCharacter>(GetAvatarActor());
	if (!BaseCharacter)
	{
		EndTask();
		return;
	}

	if (!BaseCharacter->GetExecutionTarget())
	{
		EndTask();
		return;
	}

	ABossMk* NewBoss = Cast<ABossMk>(BaseCharacter->GetExecutionTarget());
	if (!NewBoss)
	{
		EndTask();
		return;
	}

	if (NewBoss->Hp <= 0)
	{
		EndTask();
		return;
	}

	NewBoss->GetMesh()->GetAnimInstance()->StopAllMontages(false);
	NewBoss->Fsm->mState = EEnemyState::Undefined;
	
	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = true;

	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetAvatarActor()->GetWorld(), LevelSequence,
		PlaybackSettings, LevelSequenceActor);

	LevelSequencePlayer->OnFinished.AddDynamic(this, &ThisClass::OnFinish);

	BaseCharacter->GetCharacterMovement()->DisableMovement();
	
	const FRotator RotateTo = {0, BaseCharacter->GetControlRotation().Yaw + 180, 0};

	NewBoss->SetActorEnableCollision(false);
	NewBoss->SetActorLocation(BaseCharacter->GetActorForwardVector() * 30 + GetAvatarActor()->GetActorLocation());
	NewBoss->SetActorRotation(RotateTo);
	
	LevelSequenceActor->AddBindingByTag(FName(TEXT("Player")), GetAvatarActor());
	LevelSequenceActor->AddBindingByTag(FName(TEXT("Enemy")), NewBoss);
	
	LevelSequenceActor->SetActorTransform(GetAvatarActor()->GetTransform());
	
	LevelSequencePlayer->Play();
}

void UAT_PlayLevelSequence::ExternalConfirm(bool bEndTask)
{
	Super::ExternalConfirm(bEndTask);
	OnCinematicEndNotify.Broadcast();
}

void UAT_PlayLevelSequence::OnFinish()
{
	OnCinematicEndNotify.Broadcast();
}