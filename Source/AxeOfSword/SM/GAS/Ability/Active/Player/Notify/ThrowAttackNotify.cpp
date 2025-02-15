#include "ThrowAttackNotify.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/Weapon/LeviathanAxe.h"

void UThrowAttackNotify::Notify(USkeletalMeshComponent* MeshComp
								, UAnimSequenceBase* Animation
								, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(BaseCharacter))
	{
		return;
	}
	
	const UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
	ALeviathanAxe* Axe = Cast<ALeviathanAxe>(EquipComponent->GetMainWeapon());
	if (!Axe)
	{
		return;
	}

	FThrowParameter ThrowParameter;
	ThrowParameter.ThrowRotate = BaseCharacter->GetController()->GetControlRotation();
	ThrowParameter.ThrowMovePower = 1000;
	ThrowParameter.ThrowRotatePower = 1440;
	
	Axe->Throw(ThrowParameter);
}
