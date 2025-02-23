#include "GA_LeviathanAxe_Throw.h"

#include "AT_LeviathanAxe_Throw.h"

void UGA_LeviathanAxe_Throw::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AT_LeviathanAxeThrow = UAT_LeviathanAxe_Throw::InitialEvent(this, SkillBalance);
	AT_LeviathanAxeThrow->OnThrowEndNotified.AddDynamic(this, &ThisClass::OnEndThrowAxe);
	
	AT_LeviathanAxeThrow->ReadyForActivation();
}

void UGA_LeviathanAxe_Throw::EndAbility(const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility
					, bWasCancelled);
}

void UGA_LeviathanAxe_Throw::OnEndThrowAxe()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
