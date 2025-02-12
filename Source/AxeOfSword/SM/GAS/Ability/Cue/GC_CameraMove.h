#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GC_CameraMove.generated.h"

UCLASS()
class AXEOFSWORD_API UGC_CameraMove : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

protected:
	virtual void HandleGameplayCue(AActor* MyTarget,
		EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;

private:
	UPROPERTY(EditAnywhere, Category = "Option", meta = (AllowPrivateAccess = true))
	FVector MoveTo = FVector::Zero();
	
	UPROPERTY(EditAnywhere, Category = "Option", meta = (AllowPrivateAccess = true, ClampMax = 180))
	uint8 Fov = 90;
};