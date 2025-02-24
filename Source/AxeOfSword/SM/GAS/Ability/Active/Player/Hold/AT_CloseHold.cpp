#include "AT_CloseHold.h"

#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "AxeOfSword/SM/Player/AOSPlayerController.h"
#include "AxeOfSword/SM/UI/HUD/PlayerHUD.h"
#include "AxeOfSword/SM/UI/HUD/Module/ZoomWidget.h"
#include "Kismet/KismetSystemLibrary.h"

UAT_CloseHold* UAT_CloseHold::InitialEvent(UGameplayAbility* OwningAbility)
{
	UAT_CloseHold* NewTask = NewAbilityTask<UAT_CloseHold>(OwningAbility);
	NewTask->bTickingTask = true;
	
	return NewTask;
}


void UAT_CloseHold::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetAvatarActor());
	const AAOSPlayerController* PC = Player->GetController<AAOSPlayerController>();

	FVector MouseWorldLocation;
	FVector MouseWorldDirection;
	
	PC->GetPlayerHUD()->GetZoomWidget()->ToggleZoomMode(true);
	PC->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

	TArray<AActor*> IgnoreActor;
	IgnoreActor.Add(Player);

	FHitResult HitResult;

	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), MouseWorldLocation,
		MouseWorldLocation + PC->GetControlRotation().Vector() * 2000, TraceTypeQuery1,
		false, IgnoreActor, EDrawDebugTrace::None, HitResult, true))
	{
		PC->GetPlayerHUD()->GetZoomWidget()->ToggleWing(true);
	} else
	{
		PC->GetPlayerHUD()->GetZoomWidget()->ToggleWing(false);
	}
}
