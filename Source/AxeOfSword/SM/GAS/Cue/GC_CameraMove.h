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
	
	UPROPERTY(EditDefaultsOnly, Category = "Option|Camera|Move", meta = (AllowPrivateAccess = true))
	uint8 IsEnabledCameraMove:1 = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option|Camera|Fov", meta = (AllowPrivateAccess = true))
	uint8 IsEnabledCameraFov:1 = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option", meta = (AllowPrivateAccess = true))
	uint8 IsRollback:1 = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option|Camera|Move", meta = (AllowPrivateAccess = true
		, ClampMin = 0, EditCondition = "IsEnabledCameraMove"))
	float CameraMoveSpeed = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option|Camera|Fov", meta = (AllowPrivateAccess = true
		, ClampMin = 0, ClampMax = 180, EditCondition = "IsEnabledCameraFov"))
	float CameraFovSpeed = 0;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTimelineComponent> CameraMoveTimeline;
	
	UPROPERTY(EditDefaultsOnly, Category="Option|Camera|Move", meta = (AllowPrivateAccess = true, EditCondition = "IsEnabledCameraMove"))
	TObjectPtr<UCurveVector> CameraMoveTimingCurve;
	
	FOnTimelineVector CameraMoveCallback;
	
	FOnTimelineEvent CameraMoveFinish;

	UFUNCTION()
	void OnCameraMoveCallback(FVector Output);
	
	UFUNCTION()
	void OnCameraMoveFinish();
	
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