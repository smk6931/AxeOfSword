#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "LeviathanAxe.generated.h"

struct FThrowParameter
{
	FRotator ThrowRotate;
	int32 ThrowMovePower;
	int32 ThrowRotatePower;
};

UCLASS()
class AXEOFSWORD_API ALeviathanAxe : public ABaseWeapon
{
	GENERATED_BODY()

public:
	ALeviathanAxe();

	void Throw(const FThrowParameter& ThrowParameter);
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION()
	virtual void OnOverlapWeaponCollision(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;

private:
	UFUNCTION()
	void OnHitStopEnd();

	bool IsThrow = false;

	FRotator ThrowRotate;
	int32 ThrowMovePower;
	int32 ThrowRotatePower;
};
