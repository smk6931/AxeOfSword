#include "GC_CameraShake.h"
#include "LegacyCameraShake.h"
#include "Kismet/GameplayStatics.h"

bool UGC_CameraShake::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)
		->PlayerCameraManager->StartCameraShake(CameraShakeClass);
	
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
