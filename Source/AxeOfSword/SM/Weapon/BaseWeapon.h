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

	void ClearDamageStack();
	void EquipWeaponToTarget(USkeletalMeshComponent* TargetMesh);
	virtual void UpdateWeaponAttackable(const bool IsEnable);

	GETTER(TArray<TObjectPtr<UAnimMontage>>, ComboAttackAnim)
	GETTER(TObjectPtr<UAnimMontage>, HeavyAttackAnim)
	GETTER(TObjectPtr<UAnimMontage>, ThrowAttackAnim)
	GETTER(TObjectPtr<UAnimMontage>, DamagedAnim)

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> GizmoPoint;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	virtual void OnOverlapWeaponCollision(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category = "Option|Weapon", meta = (AllowPrivateAccess = true))
	int8 Damage;

	uint8 DamageStack = 1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option|Weapon", meta = (AllowPrivateAccess = true))
	uint8 MaxDamageStack = 8;
	
	FTimerHandle EndHitStopTimerHandle;
	
private:
	// TODO: 임시용으로 무기에 장착하지만 DataTable과 EquipComponent에서 무기를 감지하고
	// 그에 맞는 애니메이션 리스트를 부과해주는 것이 맞다.
	UPROPERTY(EditDefaultsOnly, Category = "Option|Weapon", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> ComboAttackAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option|Weapon", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> HeavyAttackAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option|Weapon", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ThrowAttackAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option|Weapon", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DamagedAnim;
};