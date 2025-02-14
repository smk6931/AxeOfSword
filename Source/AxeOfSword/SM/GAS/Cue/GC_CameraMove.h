#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "Components/TimelineComponent.h"
#include "GC_CameraMove.generated.h"

UCLASS()
class AXEOFSWORD_API AGC_CameraMove : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AGC_CameraMove();
	
protected:
	virtual void BeginPlay() override;
	
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

private:
	UPROPERTY()
	AActor* Target;
	
	UPROPERTY(EditAnywhere, Category = "Option", meta = (AllowPrivateAccess = true))
	float CameraMoveSpeed = 0;
	
	UPROPERTY(EditAnywhere, Category = "Option", meta = (AllowPrivateAccess = true, ClampMax = 180))
	uint8 CameraFovSpeed = 0;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTimelineComponent> CameraMoveTimeline;
	
	UPROPERTY(EditAnywhere, Category="Option", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCurveVector> CameraMoveTimingCurve;
	
	FOnTimelineVector CameraMoveCallback;
	
	FOnTimelineEvent CameraMoveFinish;

	UFUNCTION()
	void OnCameraMoveCallback(FVector Output);
	
	UFUNCTION()
	void OnCameraMoveFinish();
};