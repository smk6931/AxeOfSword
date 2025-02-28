#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayCueNotify_Static.h"
#include "GC_CameraShake.generated.h"

class ULegacyCameraShake;

UCLASS()
class AXEOFSWORD_API UGC_CameraShake : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

protected:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	TSubclassOf<ULegacyCameraShake> CameraShakeClass;
};
