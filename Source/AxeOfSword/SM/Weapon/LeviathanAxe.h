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
};
