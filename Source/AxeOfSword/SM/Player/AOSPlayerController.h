#pragma once

#include "CoreMinimal.h"
#include "AxeOfSword/SM/Helper/UtilHelper.h"
#include "GameFramework/PlayerController.h"
#include "AOSPlayerController.generated.h"


class UPlayerHUD;

UCLASS()
class AXEOFSWORD_API AAOSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UPlayerHUD>, PlayerHUD)
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|HUD", meta=(AllowPrivateAccess = true))
	TSubclassOf<UPlayerHUD> PlayerHUDClass;

	UPROPERTY()
	TObjectPtr<UPlayerHUD> PlayerHUD;
};
