#include "GA_BaseAttack.h"

#include "AbilitySystemComponent.h"
#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
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
	return UStateHelper::IsIdle(GetAvatarActorFromActorInfo());
}

void UGA_BaseAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 캐스팅 상태이면서도, 현재 CloseHold Ability가 활성화된 상태에서 공격 키를 누르면
	// 무기 던지기 특수 스킬을 구현한다.
	if (AOSGameplayTags::HasGameplayTag(GetAbilitySystemComponentFromActorInfo(),
	AOSGameplayTags::Ability_CloseHold))
	{
		const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
		if (!BaseCharacter->GetEquipComponent()->IsMainWeaponOwner())
		{
			return;
		}
		
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(AOSGameplayTags::Ability_Attack_Throw);
		
		GetAbilitySystemComponentFromActorInfo()->CancelAbilities(&TagContainer);
		GetAbilitySystemComponentFromActorInfo()->TryActivateAbilitiesByTag(TagContainer);
		
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
		return;
	}
	
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

	// 계속 눌려있을 때 Pressed가 호출되며, 홀딩이 계속되는 상태이면서도 Idle인 상태 즉
	// 일반 공격 1번이 끝났다는 것으로 정의해 강공격을 이후 계속 진행한다.
	if (IsPlayerHoldingAttack() && UStateHelper::IsIdle(GetAvatarActorFromActorInfo()))
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

bool UGA_BaseAttack::IsPlayerHoldingAttack() const
{
	return AOSGameplayTags::HasGameplayTag(GetAbilitySystemComponentFromActorInfo(),
		AOSGameplayTags::Status_Attack_Hold);
}