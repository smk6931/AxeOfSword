#include "ComboAttackNotifyState.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"

void UComboAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(BaseCharacter))
	{
		return;
	}

	UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
	EquipComponent->ToggleAttack(true);
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
	AOSGameplayTags::RemoveGameplayTag(BaseCharacter->GetAbilitySystemComponent(), AOSGameplayTags::State_Attack, 0);
} 