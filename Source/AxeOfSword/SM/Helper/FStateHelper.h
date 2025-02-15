#pragma once

class UAbilitySystemComponent;

class FStateHelper
{
public:
	UFUNCTION(BlueprintCallable)
	static bool IsIdle(const UAbilitySystemComponent* ASC);
	static void ClearState(const UAbilitySystemComponent* ASC);
};