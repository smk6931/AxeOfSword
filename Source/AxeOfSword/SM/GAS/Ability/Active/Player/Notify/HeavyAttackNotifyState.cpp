#include "HeavyAttackNotifyState.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/Helper/EnumHelper.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"
#include "AxeOfSword/SM/Helper/StateHelper.h"
#include "AxeOfSword/SM/Weapon/LeviathanAxe.h"

void UHeavyAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(BaseCharacter))
	{
		return;
	}

	UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
	BaseCharacter->SetCurrentState(ECharacterState::AttackIng);
	EquipComponent->ToggleAttack(true);

	if (const ALeviathanAxe* LeviathanAxe = Cast<ALeviathanAxe>(EquipComponent->GetMainWeapon()))
	{
		LeviathanAxe->SetPlayEffect(true);
	}
}

void UHeavyAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp,
                                        UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(BaseCharacter))
	{
		return;
	}
	
	if (const ALeviathanAxe* LeviathanAxe = Cast<ALeviathanAxe>(BaseCharacter->GetEquipComponent()->GetMainWeapon()))
	{
		LeviathanAxe->SetPlayEffect(false);
	}
	
	UStateHelper::ClearState(BaseCharacter);
}
