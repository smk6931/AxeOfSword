#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZoomWidget.generated.h"

class UOverlay;
class UImage;

UCLASS()
class AXEOFSWORD_API UZoomWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ToggleZoomMode(const bool IsOn);
	
	void ToggleWing(const bool IsOn);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Layout;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CenterPos;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> RightWing;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> LeftWing;
};
