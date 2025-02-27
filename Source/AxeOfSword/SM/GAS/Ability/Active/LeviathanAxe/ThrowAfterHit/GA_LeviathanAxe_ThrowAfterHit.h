#pragma once

#include "CoreMinimal.h"
#include "AT_LeviathanAxe_ThrowAfterHit.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/BaseInputAbility.h"
#include "GA_LeviathanAxe_ThrowAfterHit.generated.h"

UCLASS()
class AXEOFSWORD_API UGA_LeviathanAxe_ThrowAfterHit : public UBaseInputAbility
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
	TObjectPtr<UAT_LeviathanAxe_ThrowAfterHit> AT_LeviathanAxeThrow;

	UPROPERTY(EditDefaultsOnly, Category = "Option|Balance", meta = (AllowPrivateAccess = true))
	FLeviathanAxeThrowAfterHitBalance SkillBalance;

	UFUNCTION()
	void OnEndThrowAxe();
};
