#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AxeOfSword/SM/Helper/UtilHelper.h"
#include "AxeOfSword/SM/GAS/AOSAbilitySystemInitializeData.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class UWeaponAnimation;
class UBoxComponent;
class UAOSAbilitySystemComponent;

UCLASS()
class AXEOFSWORD_API ABaseWeapon : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseWeapon();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	void ClearDamageStack();
	void CastWeaponSkill(const FGameplayTag& SkillTag);
	void EquipWeaponToTarget(USkeletalMeshComponent* TargetMesh);
	virtual void UpdateWeaponAttackable(const bool IsEnable);

	void SetWeaponMeshRotation(const FRotator& NewRotator);

	GETTER(TObjectPtr<UWeaponAnimation>, WeaponAnimationData)

	GETTER(TObjectPtr<UStaticMeshComponent>, WeaponMesh)

	GETTER(int8, Damage);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	UAOSAbilitySystemComponent* AbilitySystemComponent;
	
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

	UPROPERTY(EditDefaultsOnly, Category = "Options|Weapon", meta = (AllowPrivateAccess = true))
	int8 Damage;

	uint8 DamageStack = 1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Weapon", meta = (AllowPrivateAccess = true))
	uint8 MaxDamageStack = 8;
	
	FTimerHandle EndHitStopTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options")
	TObjectPtr<UAOSAbilitySystemInitializeData> InitialData;
	
private:
	// TODO: 임시용으로 무기에 장착하지만 DataTable과 EquipComponent에서 무기를 감지하고
	// 그에 맞는 애니메이션 리스트를 부과해주는 것이 맞다.
	UPROPERTY(EditDefaultsOnly, Category = "Options|Weapon", meta = (AllowPrivateAccess = true))
	TObjectPtr<UWeaponAnimation> WeaponAnimationData;
};