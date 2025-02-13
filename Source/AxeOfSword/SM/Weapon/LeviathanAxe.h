#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "LeviathanAxe.generated.h"

UCLASS()
class AXEOFSWORD_API ALeviathanAxe : public ABaseWeapon
{
	GENERATED_BODY()

public:
	ALeviathanAxe();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapWeaponCollision(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;

private:
	FTimerHandle EndHitStopTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Option", meta = (AllowPrivateAccess = true, ClampMin = 0))
	float HitStopEndTime = 0.1f;

	UFUNCTION()
	void OnHitStopEnd();
};
