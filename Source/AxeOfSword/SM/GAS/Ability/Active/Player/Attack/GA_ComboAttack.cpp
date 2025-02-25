#include "GA_ComboAttack.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Weapon/BaseWeapon.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/Data/WeaponAnimation.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"
#include "AxeOfSword/SM/Helper/StateHelper.h"

bool UGA_ComboAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                         const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return UStateHelper::IsIdle(GetAvatarActorFromActorInfo());
}

void UGA_ComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(CurrentActorInfo->AvatarActor);
	if (!IsValid(BaseCharacter))
	{
		return;
	}
 
	const UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();

	BaseCharacter->SetCurrentState(ECharacterState::Attack);
	
	// 최초 실행 시 첫번째 Montage를 실행시킨다.
	// 캐싱은 하지 않는다. 캐싱의 경우는 무기에 따라 달라지지만
	// 현재는 해당 방식을 굳이 채용할 필요는 없다고 판단했다
	// TODO: 가능하면 Pooling 처리 해두기
	AT_ComboAttackAnim = UPlayMontageWithEvent::InitialEvent(
		this, NAME_None,
		EquipComponent->GetMainWeapon()->GetWeaponAnimationData()
			->GetComboAttackAnim()[EquipComponent->GetComboIndex()],
		FGameplayTagContainer()
		);
	AT_ComboAttackAnim->OnInterrupted.AddDynamic(this, &ThisClass::OnBlendOutAttack);
	AT_ComboAttackAnim->OnBlendOut.AddDynamic(this, &ThisClass::OnBlendOutAttack);
	AT_ComboAttackAnim->OnCancelled.AddDynamic(this, &ThisClass::OnCancelAttack);
	AT_ComboAttackAnim->OnCompleted.AddDynamic(this, &ThisClass::OnEndAttack);
	
	AT_ComboAttackAnim->Activate();
}

void UGA_ComboAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_ComboAttack::OnBlendOutAttack(FGameplayTag EventTag, FGameplayEventData EventData)
{
	GetWorld()->GetTimerManager().ClearTimer(EndDefaultAttackHandle);
	GetWorld()->GetTimerManager().SetTimer(EndDefaultAttackHandle,
		FTimerDelegate::CreateUObject(this, &ThisClass::OnEndCombo), ComboEndDelayTime, false);
	EndAbility(CurrentSpecHandle, CurrentActorInfo,
		CurrentActivationInfo, false, false);
}

void UGA_ComboAttack::OnCancelAttack(FGameplayTag EventTag, FGameplayEventData EventData)
{
	GetWorld()->GetTimerManager().ClearTimer(EndDefaultAttackHandle);
}

void UGA_ComboAttack::OnEndAttack(FGameplayTag EventTag, FGameplayEventData EventData)
{
	UStateHelper::ClearState(GetAvatarActorFromActorInfo());
}

void UGA_ComboAttack::OnEndCombo()
{
	const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(BaseCharacter))
	{
		return;
	}
	
	UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
	EquipComponent->ClearCombo();
}