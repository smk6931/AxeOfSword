#pragma once

#include "CoreMinimal.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/BaseInputAbility.h"
#include "GA_Attack.generated.h"

class UPlayMontageWithEvent;

UCLASS()
class AXEOFSWORD_API UGA_Attack : public UBaseInputAbility
{
	GENERATED_BODY()

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
		const FGameplayEventData* TriggerEventData = nullptr) override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
	bool IsHoldEnd = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	float ComboEndDelayTime = 3.f;

	UPROPERTY()
	UPlayMontageWithEvent* AT_ComboAttackAnim;

	UFUNCTION()
	void OnCancelAttack(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION()
	void OnEndAttack(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION()
	void OnEndHeavyAttack(FGameplayTag EventTag, FGameplayEventData EventData);

	FTimerHandle EndDefaultAttackHandle;

	UFUNCTION()
	void OnEndCombo();

	bool IsAvatarDoingAttack() const;
	bool IsAvatarDoingHeavyAttack() const;
};