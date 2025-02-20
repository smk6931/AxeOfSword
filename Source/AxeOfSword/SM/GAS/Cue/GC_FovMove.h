#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "Components/TimelineComponent.h"
#include "GC_FovMove.generated.h"

UCLASS()
class AXEOFSWORD_API AGC_FovMove : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AGC_FovMove();

protected:
	virtual void BeginPlay() override;
	
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

private:
	UPROPERTY()
	AActor* Target;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option", meta = (AllowPrivateAccess = true))
	uint8 IsRollback:1 = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option", meta = (AllowPrivateAccess = true, ClampMin = 0))
	float FovMoveSpeed = 0;

	bool IsReverse;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTimelineComponent> FovMoveTimeline;
	
	UPROPERTY(EditDefaultsOnly, Category="Option|Camera|Fov", meta = (AllowPrivateAccess = true, EditCondition = "IsEnabledCameraFov"))
	TObjectPtr<UCurveFloat> FovMoveTimingCurve;
	
	FOnTimelineFloat FovMoveCallback;
	
	FOnTimelineEvent FovMoveFinish;

	UFUNCTION()
	void OnFovMoveCallback(float Output);
	
	UFUNCTION()
	void OnFovMoveFinish();
};
