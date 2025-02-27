#include "ZoomWidget.h"

#include "Components/Image.h"

void UZoomWidget::ToggleZoomMode(const bool IsOn)
{
	CenterPos->SetVisibility(IsOn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	LeftWing->SetVisibility(IsOn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	RightWing->SetVisibility(IsOn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UZoomWidget::ToggleWing(const bool IsOn)
{
	LeftWing->SetVisibility(IsOn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	RightWing->SetVisibility(IsOn ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
