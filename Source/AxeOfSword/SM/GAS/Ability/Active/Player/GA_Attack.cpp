#include "GA_Attack.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "AxeOfSword/SM/Weapon/BaseWeapon.h"

void UGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                 const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	StartAttackTime = FDateTime::Now();
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(ActorInfo->AvatarActor);
	if (!IsValid(BaseCharacter))
	{
		return;
	}
	UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
	AT_ComboAttackAnim = UPlayMontageWithEvent::InitialEvent(
		this, "",
		EquipComponent->GetMainWeapon()->GetComboAttackAnim()[EquipComponent->GetComboIndex()],
		FGameplayTagContainer()
		);
}

void UGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo
	, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Attack::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if ((FDateTime::Now() - StartAttackTime).GetSeconds() >= HeavyAttackHoldTime)
	{
		UE_LOG(LogTemp, Display, TEXT("강공격"))
	} else
	{
		UE_LOG(LogTemp, Display, TEXT("일반공격"))
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}