#include "EquipComponent.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Weapon/BaseWeapon.h"


UEquipComponent::UEquipComponent()
{

}

void UEquipComponent::BeginPlay()
{
	Super::BeginPlay();
	
	MainWeapon = GetWorld()->SpawnActor<ABaseWeapon>(MainWeaponClass);
	
	if (IsFirstEquip)
	{
		ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetOwner());
		if (!IsValid(BaseCharacter))
		{
			return;
		}
		MainWeapon->EquipWeaponToTarget(BaseCharacter->GetMesh());
	} else
	{
		// TODO: 해당 부분은 나중에 무기 뽑기를 위해 Socket 위치가 변경된 상태로 처리한다.
		MainWeapon->SetHidden(true);
	}
}

void UEquipComponent::SetNextCombo()
{
	if (ComboIndex == MainWeapon->GetComboAttackAnim().Num() - 1)
	{
		ComboIndex = 0;
		return;
	}
	ComboIndex += 1;
}

void UEquipComponent::ClearCombo()
{
	ComboIndex = 0;
}

