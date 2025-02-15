#include "FStateHelper.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagHelper.h"

bool FStateHelper::IsIdle(const UAbilitySystemComponent* ASC)
{
	return ASC->HasMatchingGameplayTag(AOSGameplayTags::State_Idle);
}

void FStateHelper::ClearState(const UAbilitySystemComponent* ASC)
{
	
}
