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
	virtual void UpdateWeaponAttackable(const bool IsEnable);

	GETTER(TArray<TObjectPtr<UAnimMontage>>, ComboAttackAnim)
	GETTER(TObjectPtr<UAnimMontage>, HeavyAttackAnim)
	GETTER(TObjectPtr<UAnimMontage>, ThrowAttackAnim)

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	// 상황에 따라 LineTrace or Collision 세팅해도 좋지만 우선은 도끼만을 위해 Box Collision으로 처리해둔다.
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> AttackCollision;

	virtual void OnOverlapWeaponCollision(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	int8 Damage;
	
	FTimerHandle EndHitStopTimerHandle;
	
private:
	// TODO: 임시용으로 무기에 장착하지만 DataTable과 EquipComponent에서 무기를 감지하고
	// 그에 맞는 애니메이션 리스트를 부과해주는 것이 맞다.
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> ComboAttackAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> HeavyAttackAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ThrowAttackAnim;
};