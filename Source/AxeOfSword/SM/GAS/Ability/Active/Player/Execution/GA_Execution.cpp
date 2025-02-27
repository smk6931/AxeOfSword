#include "GA_Execution.h"

#include "LevelSequencePlayer.h"
#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AxeOfSword/SM/Data/WeaponAnimation.h"
#include "AT_PlayLevelSequence.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/Boss/BossMk.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void UGA_Execution::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
	if (!BaseCharacter)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}

	ULevelSequence* ExecutionCinematic = BaseCharacter->GetEquipComponent()
		->GetMainWeaponAnimationData()->GetExecutionCinematic();

	AT_PlayLevelSequence = UAT_PlayLevelSequence::InitialEvent(this, ExecutionCinematic);
	AT_PlayLevelSequence->OnCinematicEndNotify.AddDynamic(this, &ThisClass::OnCinematicEnd);
	AT_PlayLevelSequence->ReadyForActivation();
}

void UGA_Execution::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Execution::OnCinematicEnd()
{
	APlayerCharacter* BaseCharacter = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!BaseCharacter)
	{
		return;
	}

	if (!BaseCharacter->GetExecutionTarget())
	{
		return;
	}

	if (ABossMk* NewBoss = Cast<ABossMk>(BaseCharacter->GetExecutionTarget()))
	{
		UGameplayStatics::ApplyDamage(NewBoss, 99999,
			NewBoss->GetController(), GetAvatarActorFromActorInfo(), nullptr);
	}

	BaseCharacter->SetExecutionTarget(nullptr);
	BaseCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo,
		CurrentActivationInfo, false, false);
}