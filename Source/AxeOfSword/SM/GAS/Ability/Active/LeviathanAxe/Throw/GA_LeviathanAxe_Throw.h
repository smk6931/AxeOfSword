#pragma once

#include "CoreMinimal.h"
#include "AT_LeviathanAxe_Throw.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/BaseInputAbility.h"
#include "GA_LeviathanAxe_Throw.generated.h"

UCLASS()
class AXEOFSWORD_API UGA_LeviathanAxe_Throw : public UBaseInputAbility
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
	TObjectPtr<UAT_LeviathanAxe_Throw> AT_LeviathanAxeThrow;

	UPROPERTY(EditDefaultsOnly, Category = "Option|Balance", meta = (AllowPrivateAccess = true))
	FLeviathanAxeThrowBalance SkillBalance;
};
