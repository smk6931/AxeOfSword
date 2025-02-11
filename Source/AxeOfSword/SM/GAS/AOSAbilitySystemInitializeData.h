#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AxeOfSword/SM/Helper/UtilHelper.h"
#include "AOSAbilitySystemInitializeData.generated.h"

class UBaseInputAbility;
class UAttributeSet;

UCLASS()
class AXEOFSWORD_API UAOSAbilitySystemInitializeData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	GETTER(TArray<TSubclassOf<UBaseInputAbility>>, DefaultGameplayAbilities)
	GETTER(FGameplayTagContainer, DefaultGameplayTags)
	
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UBaseInputAbility>> DefaultGameplayAbilities;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FGameplayTagContainer DefaultGameplayTags;
};
