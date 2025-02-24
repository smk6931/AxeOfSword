#include "GA_LeviathanAxe_ThrowAfterHit.h"

#include "AbilitySystemComponent.h"
#include "AT_LeviathanAxe_ThrowAfterHit.h"
#include "AxeOfSword/SM/Weapon/LeviathanAxe.h"

void UGA_LeviathanAxe_ThrowAfterHit::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AT_LeviathanAxeThrow = UAT_LeviathanAxe_ThrowAfterHit::InitialEvent(this, SkillBalance);
	AT_LeviathanAxeThrow->OnThrowAfterHitEndNotified.AddDynamic(this, &ThisClass::OnEndThrowAxe);

	ALeviathanAxe* LeviathanAxe = Cast<ALeviathanAxe>(GetAvatarActorFromActorInfo());
	
	LeviathanAxe->UpdateWeaponAttackable(true);
	
	AT_LeviathanAxeThrow->ReadyForActivation();
}

void UGA_LeviathanAxe_ThrowAfterHit::EndAbility(const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALeviathanAxe* LeviathanAxe = Cast<ALeviathanAxe>(GetAvatarActorFromActorInfo());
	LeviathanAxe->UpdateWeaponAttackable(false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility
					, bWasCancelled);
}

void UGA_LeviathanAxe_ThrowAfterHit::OnEndThrowAxe()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}