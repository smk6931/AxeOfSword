#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "Components/TimelineComponent.h"
#include "AxeOfSword/SM/Helper/EnumHelper.h"
#include "LeviathanAxe.generated.h"

enum class ELeviathanAxeStatus : uint8;

UCLASS()
class AXEOFSWORD_API ALeviathanAxe : public ABaseWeapon
{
	GENERATED_BODY()

public:
	ALeviathanAxe();

	void Throw();

	void TurnBack(AActor* NewOwner);

	GETTER_SETTER(ELeviathanAxeStatus, AxeStatus)
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION()
	virtual void OnOverlapWeaponCollision(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> WeaponStart;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> WeaponEnd;

private:
	ELeviathanAxeStatus AxeStatus = ELeviathanAxeStatus::Idle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option|Leviathan", meta = (AllowPrivateAccess = true))
	float GravityScale = 720.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option|Leviathan",
		meta = (AllowPrivateAccess = true))
	int32 ThrowMovePower = 2000;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option|Leviathan",
		meta = (AllowPrivateAccess = true))
	int32 ThrowRotatePower = 720;
	
	float GravityStack;
	
	FRotator ThrowRotate;

	FVector TurnBackStartLocation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTimelineComponent> TurnBackTimeline;
	
	UPROPERTY(EditDefaultsOnly, Category="Option|Leviathan", meta = (AllowPrivateAccess = true, EditCondition = "IsEnabledCameraFov"))
	TObjectPtr<UCurveFloat> TurnBackTimingCurve;
	
	FOnTimelineFloat TurnBackCallback;
	
	FOnTimelineEvent TurnBackFinish;

	void OnHitThrown(const FHitResult& HitResult);
	void OnHitDamage(AActor* TargetActor);
	
	UFUNCTION()
	void OnHitStopEnd();

	UFUNCTION()
	void OnTurnBackCallback(float Output);
	
	UFUNCTION()
	void OnTurnBackFinish();

	void TraceWeaponThrow(FHitResult& HitResult);
};
