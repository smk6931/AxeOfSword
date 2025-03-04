#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "AxeOfSword/SM/Helper/EnumHelper.h"
#include "LeviathanAxe.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
enum class ELeviathanAxeState : uint8;

UCLASS()
class AXEOFSWORD_API ALeviathanAxe : public ABaseWeapon
{
	GENERATED_BODY()

public:
	ALeviathanAxe();

	void Throw();

	GETTER_SETTER(ELeviathanAxeState, AxeStatus)
	GETTER(TObjectPtr<USceneComponent>, WeaponPivot)
	GETTER(FTransform, InitialWeaponMeshTransform)

	void SetPlayThrowSound(const bool IsEnable) const;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnOverlapWeaponCollision(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> WeaponPivot;

private:
	ELeviathanAxeState AxeStatus = ELeviathanAxeState::Idle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Leviathan|Sound", meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundBase> ThrowLoopSound;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Leviathan",
		meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase> TurnBackEndCameraShake;
	
	float GravityStack;
	
	FRotator ThrowRotate;
	
	FTransform InitialWeaponMeshTransform;
	
	void OnHitDamage(AActor* TargetActor);
	
	UFUNCTION()
	void OnHitStopEnd();
};