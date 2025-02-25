#include "GA_ThrowAttack.h"

#include "AbilitySystemComponent.h"
#include "AxeOfSword/SM/Data/WeaponAnimation.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"
#include "AxeOfSword/SM/Weapon/BaseWeapon.h"
#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "AxeOfSword/SM/Helper/StateHelper.h"

bool UGA_ThrowAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                         const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_ThrowAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(CurrentActorInfo->AvatarActor);
	if (!IsValid(BaseCharacter))
	{
		return;
	}

	FGameplayTagContainer CancelTags;
	CancelTags.AddTag(AOSGameplayTags::Ability_CloseHold);
	GetAbilitySystemComponentFromActorInfo()->CancelAbilities(&CancelTags);
	
	const UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();

	AT_ThrowAttackAnim = UPlayMontageWithEvent::InitialEvent(
		this, NAME_None,
		EquipComponent->GetMainWeapon()->GetWeaponAnimationData()->GetThrowAttackAnim(),
		FGameplayTagContainer()
		);
	AT_ThrowAttackAnim->OnBlendOut.AddDynamic(this, &ThisClass::OnEndThrowAttack);
	AT_ThrowAttackAnim->OnCancelled.AddDynamic(this, &ThisClass::OnEndThrowAttack);
	AT_ThrowAttackAnim->OnCompleted.AddDynamic(this, &ThisClass::OnEndThrowAttack);
	
	AT_ThrowAttackAnim->Activate();
}

void UGA_ThrowAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UStateHelper::ClearState(GetAvatarActorFromActorInfo());
	AOSGameplayTags::RemoveGameplayTag(GetAbilitySystemComponentFromActorInfo(),
	AOSGameplayTags::Ability_Attack_Throw);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_ThrowAttack::OnEndThrowAttack(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo,
		CurrentActivationInfo, false, false);
}