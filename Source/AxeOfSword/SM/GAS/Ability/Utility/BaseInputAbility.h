#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AxeOfSword/SM/Helper/EnumHelper.h"
#include "AxeOfSword/SM/Helper/UtilHelper.h"
#include "BaseInputAbility.generated.h"

enum class EAbilityInputId : uint8;
class UInputAction;

UCLASS()
class AXEOFSWORD_API UBaseInputAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UBaseInputAbility(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, EditAnywhere
		, Category = "Custom Gameplay Ability")
	bool ActivateAbilityOnGranted = false;

	GETTER(EAbilityInputId, InputId)
	
protected:
	uint32 TriggeredEventHandle = -1;

	void SetupEnhancedInputBindings(const FGameplayAbilityActorInfo* ActorInfo
																	, const FGameplayAbilitySpec& Spec);

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo
													, const FGameplayAbilitySpec& Spec) override;

	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo
															, const FGameplayAbilitySpec& Spec) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle
													, const FGameplayAbilityActorInfo* ActorInfo
													, const FGameplayAbilityActivationInfo ActivationInfo
													, bool bReplicateEndAbility
													, bool bWasCancelled) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> ActivationInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	EAbilityInputId InputId = EAbilityInputId::Undefined;
	
	void OnAbilityInputPressed(const FGameplayAbilityActorInfo* ActorInfo);

	void OnAbilityInputReleased(const FGameplayAbilityActorInfo* ActorInfo);
};
