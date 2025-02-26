#include "AT_PlayLevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/Boss/BossMk.h"
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
		return;
	}

	if (!BaseCharacter->GetExecutionTarget())
	{
		return;
	}

	ABossMk* NewBoss = Cast<ABossMk>(BaseCharacter->GetExecutionTarget());
	if (!NewBoss)
	{
		return;
	}
	NewBoss->Fsm->mState = EEnemyState::UnKnown;
	
	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = true;

	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetAvatarActor()->GetWorld(), LevelSequence,
		PlaybackSettings, LevelSequenceActor);

	LevelSequenceActor->AddBindingByTag(FName(TEXT("Player")), GetAvatarActor());
	LevelSequenceActor->AddBindingByTag(FName(TEXT("Enemy")), NewBoss);

	const FRotator RotateTo = {0, BaseCharacter->GetControlRotation().Yaw, 0};
	
	NewBoss->SetActorLocation(BaseCharacter->GetActorForwardVector() * 30 + GetAvatarActor()->GetActorLocation());
	NewBoss->SetActorRotation(RotateTo * -1);
	
	LevelSequenceActor->SetActorTransform(GetAvatarActor()->GetTransform());
	LevelSequencePlayer->Play();
}
