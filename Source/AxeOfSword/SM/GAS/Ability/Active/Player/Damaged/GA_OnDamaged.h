#pragma once

#include "CoreMinimal.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/BaseInputAbility.h"
#include "GA_OnDamaged.generated.h"

class UPlayMontageWithEvent;

UCLASS()
class AXEOFSWORD_API UGA_OnDamaged : public UBaseInputAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UPROPERTY()
	TObjectPtr<UPlayMontageWithEvent> AT_DamagedAnim;

	UFUNCTION()
	void OnDamagedBlendOut(FGameplayTag EventTag, FGameplayEventData EventData);
};