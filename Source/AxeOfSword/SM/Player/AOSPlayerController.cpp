#include "AOSPlayerController.h"

void AAOSPlayerController::BeginPlay()
{
	ConsoleCommand(TEXT("stat unit"));
	ConsoleCommand(TEXT("showdebug abilitysystem"));
}
