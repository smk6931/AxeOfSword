#pragma once

#include "CoreMinimal.h"
#include "AxeOfSword/SM/Helper/UtilHelper.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class UZoomWidget;
class UValuePercentBar;

UCLASS()
class AXEOFSWORD_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void ChangeHealthValue(const float NewCurrentValue);

	void InitializeHUD();

	GETTER(TObjectPtr<UZoomWidget>, ZoomWidget);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UValuePercentBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UZoomWidget> ZoomWidget;
};
