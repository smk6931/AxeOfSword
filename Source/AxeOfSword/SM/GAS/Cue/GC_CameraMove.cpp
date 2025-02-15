#include "GC_CameraMove.h"

#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "AxeOfSword/SM/Character/Component/PlayerCameraComponent.h"

AGC_CameraMove::AGC_CameraMove()
{
	SetHidden(true);
	CameraMoveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Camera Move Timeline Component"));
}

void AGC_CameraMove::BeginPlay()
{
	Super::BeginPlay();

	CameraMoveCallback.BindDynamic(this, &ThisClass::OnCameraMoveCallback);
	CameraMoveFinish.BindDynamic(this, &ThisClass::OnCameraMoveFinish);
	
	CameraMoveTimeline->SetLooping(false);
	CameraMoveTimeline->AddInterpVector(CameraMoveTimingCurve, CameraMoveCallback);
	CameraMoveTimeline->SetTimelineFinishedFunc(CameraMoveFinish);
}

bool AGC_CameraMove::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (Super::OnExecute_Implementation(MyTarget, Parameters))
	{
		return true;
	}

	Target = MyTarget;

	CameraMoveTimeline->PlayFromStart();
	
	return false;
}

void AGC_CameraMove::OnCameraMoveCallback(FVector Output)
{
	const APlayerCharacter* Player = Cast<APlayerCharacter>(Target);
	if (!Player)
	{
		return;
	}

	UPlayerCameraComponent* PlayerCameraComponent = Player->GetCameraComponent();
	
	PlayerCameraComponent->SetCameraOption(
		PlayerCameraComponent->GetDefaultLocation() + Output * CameraMoveSpeed);
}
 
void AGC_CameraMove::OnCameraMoveFinish()
{
	const APlayerCharacter* Player = Cast<APlayerCharacter>(Target);
	if (!Player)
	{
		return;
	}
	
	if (IsRollback)
	{
		Player->GetCameraComponent()->RollbackToFirstCameraOption();
	}
}
