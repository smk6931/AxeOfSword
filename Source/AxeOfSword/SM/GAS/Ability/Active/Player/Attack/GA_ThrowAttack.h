﻿#pragma once

#include "CoreMinimal.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/BaseInputAbility.h"
#include "GA_ThrowAttack.generated.h"

class UPlayMontageWithEvent;

UCLASS()
class AXEOFSWORD_API UGA_ThrowAttack : public UBaseInputAbility
{
	GENERATED_BODY()
	
protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UPROPERTY()
	TObjectPtr<UPlayMontageWithEvent> AT_ThrowAttackAnim;

	UFUNCTION()
	void OnEndThrowAttack(FGameplayTag EventTag, FGameplayEventData EventData);
};
