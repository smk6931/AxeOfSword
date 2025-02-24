#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "Components/TimelineComponent.h"
#include "GC_LeviathanAxe_Shake.generated.h"

class ABaseCharacter;

UCLASS()
class AXEOFSWORD_API AGC_LeviathanAxe_Shake : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AGC_LeviathanAxe_Shake();

protected:
	virtual void BeginPlay() override;

	virtual bool OnExecute_Implementation(AActor* MyTarget,
		const FGameplayCueParameters& Parameters) override;

private:
	UPROPERTY()
	TObjectPtr<ABaseCharacter> BaseOwner;

	FRotator BaseRotation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTimelineComponent> ShakeTimeline;
	
	UPROPERTY(EditDefaultsOnly, Category="Option", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCurveVector> ShakeTimingCurve;
	
	FOnTimelineVector ShakeCallback;
	
	FOnTimelineEvent ShakeFinish;
	
	UFUNCTION()
	void OnShakeCallback(FVector Output);
	
	UFUNCTION()
	void OnShakeFinish();
};
