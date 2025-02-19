#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class UValuePercentBar;

UCLASS()
class AXEOFSWORD_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void ChangeHealthValue(const float NewCurrentValue);

	void InitializeHUD();
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UValuePercentBar> HealthBar;
};
