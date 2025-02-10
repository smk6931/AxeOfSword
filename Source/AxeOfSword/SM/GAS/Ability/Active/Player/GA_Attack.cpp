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
	if (IsAlreadyAttack)
	{
		return;
	}
	IsAlreadyAttack = true;
	StartAttackTime = FDateTime::Now();
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(ActorInfo->AvatarActor);
	if (!IsValid(BaseCharacter))
	{
		return;
	}
	
	UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
	AT_ComboAttackAnim = UPlayMontageWithEvent::InitialEvent(
		this, NAME_None,
		EquipComponent->GetMainWeapon()->GetComboAttackAnim()[EquipComponent->GetComboIndex()],
		FGameplayTagContainer()
		);
	AT_ComboAttackAnim->OnCompleted.AddDynamic(this, &ThisClass::OnEndAttack);
	
	AT_ComboAttackAnim->ReadyForActivation();
}

void UGA_Attack::OnEndAttack(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(BaseCharacter))
	{
		return;
	}
	
	UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
	EquipComponent->SetNextCombo();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false
			   , false);
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

	IsAlreadyAttack = false;
}