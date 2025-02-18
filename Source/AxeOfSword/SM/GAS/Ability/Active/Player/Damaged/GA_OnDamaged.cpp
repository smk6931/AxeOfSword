#include "GA_OnDamaged.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "AxeOfSword/SM/Weapon/BaseWeapon.h"

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
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_OnDamaged::OnDamagedBlendOut(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}