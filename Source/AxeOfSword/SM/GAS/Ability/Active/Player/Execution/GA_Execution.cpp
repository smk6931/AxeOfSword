#include "GA_Execution.h"

#include "LevelSequencePlayer.h"
#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/Data/WeaponAnimation.h"
#include "AT_PlayLevelSequence.h"

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
	
	AT_PlayLevelSequence->ReadyForActivation();
}

void UGA_Execution::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Execution::OnCinematicEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo,
		CurrentActivationInfo, false, false);
}
