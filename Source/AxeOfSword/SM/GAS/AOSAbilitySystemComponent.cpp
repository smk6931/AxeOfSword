#include "AOSAbilitySystemComponent.h"
#include "AxeOfSword/SM/GAS/Ability/BaseInputAbility.h"
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
		for (TSubclassOf<UBaseInputAbility> Ability : InitialData->GetDefaultGameplayAbilities())
		{
			UBaseInputAbility* InputAbility = Ability->GetDefaultObject<UBaseInputAbility>();

			GiveAbility(FGameplayAbilitySpec(
				Ability, InputAbility->GetAbilityLevel()
				, static_cast<uint32>(InputAbility->GetInputId()), this));
		}
	}

	if (!InitialData->GetDefaultGameplayTags().IsEmpty())
	{
		AddLooseGameplayTags(InitialData->GetDefaultGameplayTags());
	}
	
	SetIsInitialize(true);
}
