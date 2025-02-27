#include "GA_Execution.h"

#include "LevelSequencePlayer.h"
#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AxeOfSword/SM/Data/WeaponAnimation.h"
#include "AT_PlayLevelSequence.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/Boss/BossMk.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"

void UGA_Execution::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AOSGameplayTags::AddGameplayTag(GetAbilitySystemComponentFromActorInfo(), AOSGameplayTags::Status_Invincible);

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
	AOSGameplayTags::RemoveGameplayTag(GetAbilitySystemComponentFromActorInfo(),
		AOSGameplayTags::Status_Invincible);
	
	APlayerCharacter* BaseCharacter = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
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
	
	NewBoss->DiedImmediately();
	
	BaseCharacter->SetExecutionTarget(nullptr);
	BaseCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo,
		CurrentActivationInfo, false, false);
}