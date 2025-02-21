#pragma once

#include "CoreMinimal.h"
#include "AxeOfSword/SM/Helper/UtilHelper.h"
#include "Components/ActorComponent.h"
#include "EquipComponent.generated.h"

class ABaseWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXEOFSWORD_API UEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipComponent();

	GETTER(uint8, ComboIndex)
	GETTER(TObjectPtr<ABaseWeapon>, MainWeapon)
	GETTER(TObjectPtr<UAnimMontage>, ZoomMontage)
	
	void SetNextCombo();
	void ClearCombo();

	void ToggleAttack(const bool IsAttack);

	bool IsMainWeaponOwner() const;

protected:
	virtual void BeginPlay() override;

private:
	// TODO: 웬만해서는 처음에 장비하는 시스템이지만 확장성을 위해 처리함
	bool IsFirstEquip = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Weapon", meta = (AllowPrivateAccess = true))
	TSubclassOf<ABaseWeapon> MainWeaponClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Weapon", meta = (AllowPrivateAccess = true))
	TSubclassOf<ABaseWeapon> SubWeaponClass;
	
	UPROPERTY()
	TObjectPtr<ABaseWeapon> MainWeapon;

	UPROPERTY()
	TObjectPtr<ABaseWeapon> SubWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Animation", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ZoomMontage;

	uint8 ComboIndex = 0;
};