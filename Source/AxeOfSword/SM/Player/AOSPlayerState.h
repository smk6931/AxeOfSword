﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "AxeOfSword/SM/Helper/UtilHelper.h"
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
	GETTER(TObjectPtr<UBaseAttribute>, Attribute)

protected:
	virtual void BeginPlay() override;
	
private:
	bool IsCombatMode = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option", meta = (AllowPrivateAccess = true))
	float CombatEndCooldown = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option", meta = (AllowPrivateAccess = true))
	float DefaultCameraLag = 11.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option", meta = (AllowPrivateAccess = true))
	float CombatCameraLag = 3.f;
	
	TObjectPtr<UAOSAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UBaseAttribute> Attribute;
	
	FTimerHandle CombatEndTimerHandle;

	void OnCombat(const FGameplayTag CallbackTag, int32 NewCount);

	UFUNCTION()
	void OnCombatEnd();
};
