#include "EquipComponent.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Data/WeaponAnimation.h"
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
		MainWeapon->SetOwner(GetOwner());
		MainWeapon->EquipWeaponToTarget(BaseCharacter->GetMesh());
	} else
	{
		// TODO: 해당 부분은 나중에 무기 뽑기를 위해 Socket 위치가 변경된 상태로 처리한다.
		MainWeapon->SetHidden(true);
	}
}

TObjectPtr<UWeaponAnimation> UEquipComponent::GetMainWeaponAnimationData() const
{
	if (!IsMainWeaponOwner())
	{
		return FistAnimation;
	}
	
	return MainWeapon->GetWeaponAnimationData();
}

void UEquipComponent::SetNextCombo()
{
	UE_LOG(LogTemp, Display, TEXT("하이: %d"), ComboIndex)
	if (ComboIndex >= GetMainWeaponAnimationData()->GetComboAttackAnim().Num() - 1)
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

void UEquipComponent::ToggleAttack(const bool IsAttack)
{
	if (MainWeapon->GetOwner() == GetOwner())
	{
		MainWeapon->UpdateWeaponAttackable(IsAttack);
		return;
	}

	// 여기에는 주먹 관련 로직 추가
}

bool UEquipComponent::IsMainWeaponOwner() const
{
	return MainWeapon->GetOwner() == GetOwner();
}
