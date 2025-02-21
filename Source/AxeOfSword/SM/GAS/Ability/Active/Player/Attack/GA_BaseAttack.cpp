#include "GA_BaseAttack.h"

#include "AbilitySystemComponent.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"
#include "AxeOfSword/SM/Helper/StateHelper.h"

bool UGA_BaseAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                        const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	return AOSGameplayTags::HasGameplayTag(GetAbilitySystemComponentFromActorInfo(), AOSGameplayTags::State_Idle);
}

void UGA_BaseAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AOSGameplayTags::ToggleGameplayTag(
		GetAbilitySystemComponentFromActorInfo(), AOSGameplayTags::Status_Attack_Hold);

	// 호출 처음 한번만에 대해서 기본 공격 Ability를 수행해준다.
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AOSGameplayTags::Ability_Attack_Default);

	GetAbilitySystemComponentFromActorInfo()->CancelAbilities(&TagContainer);
	GetAbilitySystemComponentFromActorInfo()->TryActivateAbilitiesByTag(TagContainer);
}

void UGA_BaseAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_BaseAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	if (AOSGameplayTags::HasGameplayTag(GetAbilitySystemComponentFromActorInfo(),
		AOSGameplayTags::Status_Attack_Hold) && UStateHelper::IsIdle(GetAbilitySystemComponentFromActorInfo()))
	{
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(AOSGameplayTags::Ability_Attack_Heavy);
		GetAbilitySystemComponentFromActorInfo()->CancelAbilities(&TagContainer);
		GetAbilitySystemComponentFromActorInfo()->TryActivateAbilitiesByTag(TagContainer);
	}
}

void UGA_BaseAttack::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	AOSGameplayTags::RemoveGameplayTag(
		GetAbilitySystemComponentFromActorInfo(), AOSGameplayTags::Status_Attack_Hold);
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}