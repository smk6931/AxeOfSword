#include "GC_CameraMove.h"

#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "AxeOfSword/SM/Character/Component/PlayerCameraComponent.h"

void UGC_CameraMove::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType,
                                       const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(MyTarget, EventType, Parameters);

	APlayerCharacter* Player = Cast<APlayerCharacter>(MyTarget);
	if (!Player)
	{
		return;
	}

	if (MoveTo == FVector::Zero())
	{
		Player->GetCameraComponent()->RollbackToFirstCameraOption();
		return;
	}

	Player->GetCameraComponent()->MoveCameraOption(MoveTo, Fov);
}