#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GC_LaunchNotify.generated.h"

UCLASS()
class AXEOFSWORD_API UGC_LaunchNotify : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

protected:
	virtual void HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Option", meta = (AllowPrivateAccess = true))
	FVector LaunchTo = {500, 0, 5};
};
