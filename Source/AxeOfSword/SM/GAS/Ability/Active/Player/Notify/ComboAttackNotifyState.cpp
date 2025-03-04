#include "ComboAttackNotifyState.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/Helper/EnumHelper.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"
#include "AxeOfSword/SM/Helper/StateHelper.h"
#include "AxeOfSword/SM/Weapon/LeviathanAxe.h"

void UComboAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(BaseCharacter))
	{
		return;
	}

	BaseCharacter->SetCurrentState(ECharacterState::AttackIng);
	UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
	EquipComponent->ToggleAttack(true);

	if (const ALeviathanAxe* LeviathanAxe = Cast<ALeviathanAxe>(EquipComponent->GetMainWeapon()))
	{
		LeviathanAxe->SetPlayEffect(true);
	}
}

void UComboAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp,
                                        UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(BaseCharacter))
	{
		return;
	}
	
	UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
	EquipComponent->ToggleAttack(false);
	EquipComponent->SetNextCombo();

	if (const ALeviathanAxe* LeviathanAxe = Cast<ALeviathanAxe>(EquipComponent->GetMainWeapon()))
	{
		LeviathanAxe->SetPlayEffect(false);
	}
	
	UStateHelper::ClearState(BaseCharacter);
	AOSGameplayTags::RemoveGameplayTag(BaseCharacter->GetAbilitySystemComponent(),
		AOSGameplayTags::Status_Combat);
}