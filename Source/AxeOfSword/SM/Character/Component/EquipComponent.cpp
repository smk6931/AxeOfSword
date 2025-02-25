#include "EquipComponent.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Data/WeaponAnimation.h"
#include "AxeOfSword/SM/Weapon/BaseWeapon.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


UEquipComponent::UEquipComponent()
{

}

void UEquipComponent::BeginPlay()
{
	Super::BeginPlay();
	
	MainWeapon = GetWorld()->SpawnActor<ABaseWeapon>(MainWeaponClass);
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetOwner());
	
	if (IsFirstEquip)
	{
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

	BaseCharacter->GetFistRightSphereCapsule()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnFistAttack);
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

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetOwner());
	if (!BaseCharacter)
	{
		return;
	}
	BaseCharacter->ToggleFistAttackMode(IsAttack);
}

bool UEquipComponent::IsMainWeaponOwner() const
{
	return MainWeapon->GetOwner() == GetOwner();
}

void UEquipComponent::OnFistAttack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner())
	{
		return;
	}
	
	UGameplayStatics::ApplyDamage(OtherActor, FistDamage,
		GetOwner()->GetInstigatorController(), GetOwner(), nullptr);
}
