#include "ComboAttackNotifyState.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"

void UComboAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(BaseCharacter))
	{
		return;
	}
	
	UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
	EquipComponent->SetCanAttackNext(true);
}
