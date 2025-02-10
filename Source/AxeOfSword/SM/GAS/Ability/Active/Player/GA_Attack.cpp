#include "GA_Attack.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "AxeOfSword/SM/Weapon/BaseWeapon.h"

void UGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                 const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData* TriggerEventData)
{
	// 최초 실행
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(ActorInfo->AvatarActor);
	if (!IsValid(BaseCharacter))
	{
		return;
	}

	UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();

	// 지금이 홀딩 상태임을 의미
	IsHoldEnd = false;
	
	// 최초 실행 시 첫번째 Montage를 실행시킨다.
	AT_ComboAttackAnim = UPlayMontageWithEvent::InitialEvent(
		this, NAME_None,
		EquipComponent->GetMainWeapon()->GetComboAttackAnim()[EquipComponent->GetComboIndex()],
		FGameplayTagContainer()
		);
	AT_ComboAttackAnim->OnCompleted.AddDynamic(this, &ThisClass::OnEndAttack);
	AT_ComboAttackAnim->ReadyForActivation();
}

void UGA_Attack::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	IsHoldEnd = true;
	IsAttackEnd = false;
	UE_LOG(LogTemp, Display, TEXT("Released"))
	EndAbility(CurrentSpecHandle, CurrentActorInfo,
			CurrentActivationInfo, false, false);
}

void UGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo
	, bool bReplicateEndAbility, bool bWasCancelled)
{
	IsHoldEnd = true;
	IsAttackEnd = false;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Attack::OnCancelAttack(FGameplayTag EventTag, FGameplayEventData EventData)
{
	GetWorld()->GetTimerManager().ClearTimer(EndDefaultAttackHandle);
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

	// 현재 공격이 종료되었음을 의미한다.
	IsAttackEnd = true;
	
	GetWorld()->GetTimerManager().ClearTimer(EndDefaultAttackHandle);
	GetWorld()->GetTimerManager().SetTimer(EndDefaultAttackHandle,
		FTimerDelegate::CreateUObject(this, &ThisClass::OnEndCombo), ComboEndDelayTime, false);
}

void UGA_Attack::OnEndCombo()
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(BaseCharacter))
	{
		return;
	}
	UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
	EquipComponent->ClearCombo();
}