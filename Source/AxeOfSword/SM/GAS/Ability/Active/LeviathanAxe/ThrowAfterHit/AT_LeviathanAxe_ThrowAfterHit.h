#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_LeviathanAxe_ThrowAfterHit.generated.h"

class ALeviathanAxe;

USTRUCT()
struct FLeviathanAxeThrowAfterHitBalance
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float ThrowRotatePower;
	UPROPERTY(EditDefaultsOnly)
	float ThrowMovePower;
	UPROPERTY(EditDefaultsOnly)
	float GravityScale;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnThrowAfterHitEndNotified);

UCLASS()
class AXEOFSWORD_API UAT_LeviathanAxe_ThrowAfterHit : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	static UAT_LeviathanAxe_ThrowAfterHit* InitialEvent(UGameplayAbility* OwningAbility, const FLeviathanAxeThrowAfterHitBalance& NewBalance);
	
	virtual void Activate() override;

	FOnThrowAfterHitEndNotified OnThrowAfterHitEndNotified;
	
protected:
	virtual void TickTask(float DeltaTime) override;

private:
	TObjectPtr<ALeviathanAxe> LeviathanAxe;
	
	FRotator ThrowRotate = FRotator::ZeroRotator;
	FRotator WeaponMeshRotate = FRotator::ZeroRotator;

	FLeviathanAxeThrowAfterHitBalance Balance;

	float GravityStack = 0;

	void RotateByPowerInTick(const float DeltaTime);
	
	void TraceWeaponThrow(FHitResult& HitResult);
	void OnHitThrown(const FHitResult& HitResult);
};
