#pragma once

#include "CoreMinimal.h"
#include "AxeOfSword/SM/Helper/UtilHelper.h"
#include "AxeOfSword/SM/Weapon/BaseWeapon.h"
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

	// 이 말이 즉 내가 손에 무기를 들고있는가를 의미한다.
	// 맨손인가 아닌가에 대한 여부를 여기서 검증할 수 있음
	UFUNCTION(BlueprintCallable)
	bool IsMainWeaponOwner() const;

TObjectPtr<UWeaponAnimation> GetMainWeaponAnimationData() const;

protected:
	virtual void BeginPlay() override;

private:
	// TODO: 웬만해서는 처음에 장비하는 시스템이지만 확장성을 위해 처리함
	bool IsFirstEquip = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Weapon", meta = (AllowPrivateAccess = true))
	TSubclassOf<ABaseWeapon> MainWeaponClass;
	
	UPROPERTY()
	TObjectPtr<ABaseWeapon> MainWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Animation", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ZoomMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Animation", meta = (AllowPrivateAccess = true))
	TObjectPtr<UWeaponAnimation> FistAnimation;

	uint8 ComboIndex = 0;
};