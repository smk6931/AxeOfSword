#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AbilitySystemComponent.h"
#include "AxeOfSword/SM/Helper/UtilHelper.h"
#include "AOSAbilitySystemComponent.generated.h"

class UAOSAbilitySystemInitializeData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXEOFSWORD_API UAOSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UAOSAbilitySystemComponent();

	void Initialize(const UAOSAbilitySystemInitializeData* InitialData);

	GETTER_SETTER(bool, IsInitialize)
	
private:
	bool IsInitialize = false;
};