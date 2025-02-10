#pragma once

#include "CoreMinimal.h"
#include "AxeOfSword/SM/Helper/UtilHelper.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class UBoxComponent;

UCLASS()
class AXEOFSWORD_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ABaseWeapon();

	void EquipWeaponToTarget(USkeletalMeshComponent* TargetMesh);

	GETTER(TArray<TObjectPtr<UAnimMontage>>, ComboAttackAnim)

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	int8 Damage;

	// TODO: 임시용으로 무기에 장착하지만 DataTable과 EquipComponent에서 무기를 감지하고
	// 그에 맞는 애니메이션 리스트를 부과해주는 것이 맞다.
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> ComboAttackAnim; 
};