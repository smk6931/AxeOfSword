#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipComponent.generated.h"

class ABaseWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXEOFSWORD_API UEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipComponent();

protected:
	virtual void BeginPlay() override;

private:
	// TODO: 웬만해서는 처음에 장비하는 시스템이지만 확장성을 위해 처리함
	bool IsFirstEquip = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	TSubclassOf<ABaseWeapon> MainWeaponClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	TSubclassOf<ABaseWeapon> SubWeaponClass;
	
	UPROPERTY()
	TObjectPtr<ABaseWeapon> MainWeapon;

	UPROPERTY()
	TObjectPtr<ABaseWeapon> SubWeapon;
};