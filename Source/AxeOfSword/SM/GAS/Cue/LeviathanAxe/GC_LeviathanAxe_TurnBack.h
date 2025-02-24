#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "Components/TimelineComponent.h"
#include "GC_LeviathanAxe_TurnBack.generated.h"

class ABaseCharacter;

UCLASS()
class AXEOFSWORD_API AGC_LeviathanAxe_TurnBack : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AGC_LeviathanAxe_TurnBack();

protected:
	virtual void BeginPlay() override;

	virtual bool OnExecute_Implementation(AActor* MyTarget,
		const FGameplayCueParameters& Parameters) override;
	
private:
	FVector TurnBackStartLocation;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option",
		meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase> TurnBackEndCameraShake;
	
	UPROPERTY()
	TObjectPtr<ABaseCharacter> TargetCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Option",
		meta = (AllowPrivateAccess = true))
	int32 TurnBackRightPower = 360;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTimelineComponent> TurnBackTimeline;
	
	UPROPERTY(EditDefaultsOnly, Category="Option", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCurveVector> TurnBackTimingCurve;
	
	FOnTimelineVector TurnBackCallback;
	
	FOnTimelineEvent TurnBackFinish;

	void OnTurnBackEnd() const;

	void InitializeWeapon();
	
	UFUNCTION()
	void OnTurnBackCallback(FVector Output);
	
	UFUNCTION()
	void OnTurnBackFinish();
};