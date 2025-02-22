#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_LeviathanAxe_Throw.generated.h"

class ALeviathanAxe;

USTRUCT()
struct FLeviathanAxeThrowBalance
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float ThrowRotatePower;
	UPROPERTY(EditDefaultsOnly)
	float ThrowMovePower;
	UPROPERTY(EditDefaultsOnly)
	float GravityScale;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnThrowEndNotified);

UCLASS()
class AXEOFSWORD_API UAT_LeviathanAxe_Throw : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	static UAT_LeviathanAxe_Throw* InitialEvent(UGameplayAbility* OwningAbility, const FLeviathanAxeThrowBalance& NewBalance);
	
	virtual void Activate() override;

	FOnThrowEndNotified OnThrowEndNotified;
	
protected:
	virtual void TickTask(float DeltaTime) override;

private:
	TObjectPtr<ALeviathanAxe> LeviathanAxe;
	
	FRotator ThrowRotate = FRotator::ZeroRotator;
	FRotator WeaponMeshRotate = FRotator::ZeroRotator;

	FLeviathanAxeThrowBalance Balance;

	float GravityStack = 0;

	void RotateByPowerInTick(const float DeltaTime);
	
	void TraceWeaponThrow(FHitResult& HitResult);
	void OnHitThrown(const FHitResult& HitResult);
};
