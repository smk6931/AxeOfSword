#pragma once

#include "CoreMinimal.h"
#include "StateHelper.generated.h"

class ABaseCharacter;
class UAbilitySystemComponent;

UCLASS()
class AXEOFSWORD_API UStateHelper : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static bool IsIdle(const AActor* Target);
	
	UFUNCTION(BlueprintCallable)
	static bool IsDamaged(const AActor* Target);
	
	UFUNCTION(BlueprintCallable)
	static bool IsZoomIn(const UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable)
	static bool IsWaitForTurnBackWeapon(const AActor* Target);
	
	static void ClearState(AActor* Target);
};