#include "GA_OnDamaged.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"
#include "AxeOfSword/SM/Helper/StateHelper.h"
#include "AxeOfSword/SM/Weapon/BaseWeapon.h"

bool UGA_OnDamaged::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	
	return !UStateHelper::IsDamaged(GetAbilitySystemComponentFromActorInfo()) &&
		!GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(AOSGameplayTags::State_Attack_Ing);
}

void UGA_OnDamaged::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo,
                                FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	AOSGameplayTags::SwapGameplayTag(GetAbilitySystemComponentFromActorInfo(),
		AOSGameplayTags::State_Idle, AOSGameplayTags::State_Damaged);
}

void UGA_OnDamaged::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(CurrentActorInfo->AvatarActor);
	if (!IsValid(BaseCharacter))
	{
		return;
	}

	UAnimMontage* DamagedMontage = BaseCharacter->GetEquipComponent()->GetMainWeapon()->GetDamagedAnim();
	AT_DamagedAnim = UPlayMontageWithEvent::InitialEvent(
	this, NAME_None, DamagedMontage,
		FGameplayTagContainer()
	);
	AT_DamagedAnim->OnBlendOut.AddDynamic(this, &ThisClass::OnDamagedBlendOut);
	AT_DamagedAnim->ReadyForActivation();
}

void UGA_OnDamaged::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UStateHelper::ClearState(GetAbilitySystemComponentFromActorInfo());
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_OnDamaged::OnDamagedBlendOut(FGameplayTag EventTag, FGameplayEventData EventData)
{
	UStateHelper::ClearState(GetAbilitySystemComponentFromActorInfo());
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}