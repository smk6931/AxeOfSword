#include "GA_HeavyAttack.h"

#include "AxeOfSword/SM/Weapon/BaseWeapon.h"
#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/Data/WeaponAnimation.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"
#include "AxeOfSword/SM/Helper/StateHelper.h"

bool UGA_HeavyAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                         const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	
	return AOSGameplayTags::HasGameplayTag(GetAbilitySystemComponentFromActorInfo(), AOSGameplayTags::Status_Attack_Hold)
		&& AOSGameplayTags::HasGameplayTag(GetAbilitySystemComponentFromActorInfo(), AOSGameplayTags::State_Idle);
}

void UGA_HeavyAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AOSGameplayTags::SwapGameplayTag(GetAbilitySystemComponentFromActorInfo(),
		AOSGameplayTags::State_Idle, AOSGameplayTags::State_Attack);
	
	if (!IsValid(AT_HeavyAttackAnim))
	{
		const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(CurrentActorInfo->AvatarActor);
		if (!IsValid(BaseCharacter))
		{
			return;
		}
		
		const UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
		AT_HeavyAttackAnim = UPlayMontageWithEvent::InitialEvent(
			this, NAME_None,
			EquipComponent->GetMainWeapon()->GetWeaponAnimationData()->GetHeavyAttackAnim(),
			FGameplayTagContainer()
			);
		AT_HeavyAttackAnim->OnBlendOut.AddDynamic(this, &ThisClass::OnBlendOutHeavyAttack);
		AT_HeavyAttackAnim->OnCompleted.AddDynamic(this, &ThisClass::OnEndHeavyAttack);
	}
	AT_HeavyAttackAnim->Activate();
}

void UGA_HeavyAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UStateHelper::ClearState(GetAbilitySystemComponentFromActorInfo());
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_HeavyAttack::OnBlendOutHeavyAttack(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
		false, false);
}

void UGA_HeavyAttack::OnEndHeavyAttack(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
		false, false);
}
