#include "GA_Sprint.h"

#include "AbilitySystemComponent.h"
#include "AxeOfSword/SM/Helper/StateHelper.h"

bool UGA_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
                                    FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// TODO: 현재는 복귀 시지만 막는 전용 태그가 필요할 것 같다.
	return !UStateHelper::IsWaitForTurnBackWeapon(GetAbilitySystemComponentFromActorInfo());
}

void UGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle
                                 , const FGameplayAbilityActorInfo* ActorInfo
                                 , const FGameplayAbilityActivationInfo ActivationInfo
                                 , const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (BuffEffect)
	{
		FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponentFromActorInfo()->
			MakeEffectContext();
		EffectContext.AddSourceObject(GetAvatarActorFromActorInfo());
	
		FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
			BuffEffect, 1.0f, EffectContext);
		
		if (SpecHandle.IsValid())
		{
			ActiveHandle = GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(
				*SpecHandle.Data.Get());
		}
	}
}

void UGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility
	, bWasCancelled);
	
	GetAbilitySystemComponentFromActorInfo()->
		RemoveActiveGameplayEffectBySourceEffect(BuffEffect, GetAbilitySystemComponentFromActorInfo());
}

void UGA_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
					false, false);
}
