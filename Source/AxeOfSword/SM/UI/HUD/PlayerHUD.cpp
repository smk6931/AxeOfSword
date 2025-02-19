#include "PlayerHUD.h"

#include "AxeOfSword/SM/GAS/Attribute/BaseAttribute.h"
#include "AxeOfSword/SM/Player/AOSPlayerState.h"
#include "AxeOfSword/SM/UI/Module/ValuePercentBar.h"

void UPlayerHUD::InitializeHUD()
{
	const AAOSPlayerState* PS = GetOwningPlayerState<AAOSPlayerState>();
	if (!PS)
	{
		return;
	}

	HealthBar->SetCurrentValue(PS->GetAttribute()->GetHealth());
	HealthBar->SetMaxValue(PS->GetAttribute()->GetMaxHealth());
}

void UPlayerHUD::ChangeHealthValue(const float NewCurrentValue)
{
	HealthBar->SetCurrentValue(NewCurrentValue);
}