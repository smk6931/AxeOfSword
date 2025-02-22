#include "GA_LeviathanAxe_Throw.h"

#include "AT_LeviathanAxe_Throw.h"

void UGA_LeviathanAxe_Throw::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!AT_LeviathanAxeThrow)
	{
		AT_LeviathanAxeThrow = UAT_LeviathanAxe_Throw::InitialEvent(this, SkillBalance);
	}
	
	AT_LeviathanAxeThrow->Activate();
}

void UGA_LeviathanAxe_Throw::EndAbility(const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility
					, bWasCancelled);
}
