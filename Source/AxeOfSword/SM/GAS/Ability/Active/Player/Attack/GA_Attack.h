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

	bool IsHoldToThrow = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	float ComboEndDelayTime = 3.f;

	UPROPERTY()
	TObjectPtr<UPlayMontageWithEvent> AT_ComboAttackAnim;
	UPROPERTY()
	TObjectPtr<UPlayMontageWithEvent> AT_HeavyAttackAnim;
	UPROPERTY()
	TObjectPtr<UPlayMontageWithEvent> AT_ThrowAttackAnim;

	void ClearAttackTag();
	
	void SetCombatMode();
	
	void DoComboAttack();
	
	void DoHeavyAttack();
	
	void DoThrowAttack();

	UFUNCTION()
	void OnBlendOutAttack(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION()
	void OnCancelAttack(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION()
	void OnEndAttack(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION()
	void OnBlendOutHeavyAttack(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION()
	void OnEndHeavyAttack(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION()
	void OnEndThrowAttack(FGameplayTag EventTag, FGameplayEventData EventData);

	FTimerHandle EndDefaultAttackHandle;

	UFUNCTION()
	void OnEndCombo();

	void ClearAttackStackInWeapon();

	bool IsAvatarDoingAttack() const;
	bool IsAvatarDoingHeavyAttack() const;
};