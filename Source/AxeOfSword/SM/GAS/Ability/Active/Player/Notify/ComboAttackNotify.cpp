#include "ComboAttackNotify.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"

void UComboAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(BaseCharacter))
	{
		return;
	}
	
	UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
	EquipComponent->SetNextCombo();
}
