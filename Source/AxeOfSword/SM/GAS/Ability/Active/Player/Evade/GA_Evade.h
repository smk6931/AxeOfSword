#pragma once

#include "CoreMinimal.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/BaseInputAbility.h"
#include "GA_Evade.generated.h"

class UPlayMontageWithEvent;
enum class EMoveDirection : uint8;

UCLASS()
class AXEOFSWORD_API UGA_Evade : public UBaseInputAbility
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
	TObjectPtr<UPlayMontageWithEvent> EvadeAnim;
	
	UFUNCTION()
	void OnEndEvade(FGameplayTag EventTag, FGameplayEventData EventData);
	
	// TODO: Helper로 옮겨도 무방함
	EMoveDirection GetDirection() const;
};
