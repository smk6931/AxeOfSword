#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AxeOfSword/SM/GAS/AOSAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UEquipComponent;

UCLASS()
class AXEOFSWORD_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAOSAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UEquipComponent> EquipComponent;
};
