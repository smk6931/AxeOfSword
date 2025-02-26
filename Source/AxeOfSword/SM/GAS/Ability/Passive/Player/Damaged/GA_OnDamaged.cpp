#include "GA_OnDamaged.h"

#include "AbilitySystemComponent.h"
#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Data/WeaponAnimation.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/PlayMontageWithEvent.h"
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

	const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
	if (!BaseCharacter)
	{
		return false;
	}

	// 공격 중인 경우는 무시해도 된다.
	return !UStateHelper::IsDamaged(GetAvatarActorFromActorInfo())
		&& BaseCharacter->GetCurrentState() != ECharacterState::AttackIng;
}

void UGA_OnDamaged::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo,
                                FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
	if (!BaseCharacter)
	{
		return;
	}

	BaseCharacter->SetCurrentState(ECharacterState::Damaged);
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

	UAnimMontage* DamagedMontage = BaseCharacter->GetEquipComponent()->GetMainWeapon()->
		GetWeaponAnimationData()->GetDamagedAnim().FindRef(EMoveDirection::Forward);
	AT_DamagedAnim = UPlayMontageWithEvent::InitialEvent(
	this, NAME_None, DamagedMontage,
		FGameplayTagContainer()
	);
	AT_DamagedAnim->OnBlendOut.AddDynamic(this, &ThisClass::OnDamagedBlendOut);
	AT_DamagedAnim->OnCancelled.AddDynamic(this, &ThisClass::OnDamagedBlendOut);
	AT_DamagedAnim->OnCompleted.AddDynamic(this, &ThisClass::OnDamagedBlendOut);
	AT_DamagedAnim->OnInterrupted.AddDynamic(this, &ThisClass::OnDamagedBlendOut);
	AT_DamagedAnim->ReadyForActivation();
}

void UGA_OnDamaged::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UStateHelper::ClearState(GetAvatarActorFromActorInfo());
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_OnDamaged::OnDamagedBlendOut(FGameplayTag EventTag, FGameplayEventData EventData)
{
	UStateHelper::ClearState(GetAvatarActorFromActorInfo());
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}