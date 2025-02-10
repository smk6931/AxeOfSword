#include "AOSAbilitySystemComponent.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/BaseInputAbility.h"
#include "AOSAbilitySystemInitializeData.h"

UAOSAbilitySystemComponent::UAOSAbilitySystemComponent()
{
	
}

void UAOSAbilitySystemComponent::Initialize()
{
	if (GetIsInitialize())
	{
		return;
	}

	if (!InitialData->GetDefaultGameplayAbilities().IsEmpty())
	{
		for (auto Ability : InitialData->GetDefaultGameplayAbilities())
		{
			UBaseInputAbility* InputAbility = Ability->GetDefaultObject<UBaseInputAbility>();
			GiveAbility(FGameplayAbilitySpec(
				Ability, InputAbility->GetAbilityLevel()
				, static_cast<uint8>(InputAbility->GetInputId()), this));
		}
	}

	if (!InitialData->GetDefaultGameplayTags().IsEmpty())
	{
		AddLooseGameplayTags(InitialData->GetDefaultGameplayTags());
	}
	
	SetIsInitialize(true);
}
