#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AOSPlayerState.generated.h"

struct FGameplayTag;
struct FOnAttributeChangeData;
class UAOSAbilitySystemComponent;
class UBaseAttribute;

UCLASS()
class AXEOFSWORD_API AAOSPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAOSPlayerState();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UBaseAttribute* GetAttributeSetBase() const;
	
	float GetHealth() const;

protected:
	virtual void BeginPlay() override;
	
private:
	bool IsCombatMode = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option", meta = (AllowPrivateAccess = true))
	float CombatEndCooldown = 5.f;
	
	TObjectPtr<UAOSAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UBaseAttribute> Attribute;
	
	FDelegateHandle HealthChangedDelegateHandle;

	FTimerHandle CombatEndTimerHandle;

	void OnHealthChanged(const FOnAttributeChangeData& Data);

	void OnCombat(const FGameplayTag CallbackTag, int32 NewCount);

	UFUNCTION()
	void OnCombatEnd();
};
