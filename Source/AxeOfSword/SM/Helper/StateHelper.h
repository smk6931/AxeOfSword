#pragma once

#include "CoreMinimal.h"
#include "StateHelper.generated.h"

class UAbilitySystemComponent;

UCLASS()
class AXEOFSWORD_API UStateHelper : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static bool IsIdle(const UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable)
	static bool IsDamaged(const UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable)
	static bool IsZoomIn(const UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable)
	static bool IsWaitForTurnBackWeapon(const UAbilitySystemComponent* ASC);
	
	static void ClearState(UAbilitySystemComponent* ASC);
};