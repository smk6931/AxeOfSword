#include "AOSPlayerController.h"

#include "AxeOfSword/SM/UI/HUD/PlayerHUD.h"
#include "Blueprint/UserWidget.h"

void AAOSPlayerController::BeginPlay()
{
	// ConsoleCommand(TEXT("stat unit"));
	ConsoleCommand(TEXT("showdebug abilitysystem"));

	PlayerHUD = CreateWidget<UPlayerHUD>(this, PlayerHUDClass);
	PlayerHUD->AddToViewport();
}
