#include "GC_CameraMove.h"

#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "AxeOfSword/SM/Character/Component/PlayerCameraComponent.h"

AGC_CameraMove::AGC_CameraMove()
{
	SetHidden(true);
	CameraMoveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Camera Move Timeline Component"));
	FovMoveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Fov Move Timeline Component"));
}

void AGC_CameraMove::BeginPlay()
{
	Super::BeginPlay();

	if (IsEnabledCameraMove)
	{
		CameraMoveCallback.BindDynamic(this, &ThisClass::OnCameraMoveCallback);
		CameraMoveFinish.BindDynamic(this, &ThisClass::OnCameraMoveFinish);
	
		CameraMoveTimeline->SetLooping(false);
		CameraMoveTimeline->AddInterpVector(CameraMoveTimingCurve, CameraMoveCallback);
		CameraMoveTimeline->SetTimelineFinishedFunc(CameraMoveFinish);
	}

	if (IsEnabledCameraFov)
	{
		FovMoveCallback.BindDynamic(this, &ThisClass::AGC_CameraMove::OnFovMoveCallback);
		FovMoveFinish.BindDynamic(this, &ThisClass::AGC_CameraMove::OnFovMoveFinish);
	
		FovMoveTimeline->SetLooping(false);
		FovMoveTimeline->AddInterpFloat(FovMoveTimingCurve, FovMoveCallback);
		FovMoveTimeline->SetTimelineFinishedFunc(FovMoveFinish);
	}
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

void AGC_CameraMove::OnFovMoveCallback(float Output)
{
	const APlayerCharacter* Player = Cast<APlayerCharacter>(Target);
	if (!Player)
	{
		return;
	}

	UPlayerCameraComponent* PlayerCameraComponent = Player->GetCameraComponent();
	
	PlayerCameraComponent->SetCameraOption(
		PlayerCameraComponent->GetRelativeLocation(),
		PlayerCameraComponent->GetDefaultFov() + Output * CameraFovSpeed);
}

void AGC_CameraMove::OnFovMoveFinish()
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
