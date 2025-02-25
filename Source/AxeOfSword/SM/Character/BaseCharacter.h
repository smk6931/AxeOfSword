#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AxeOfSword/SM/Helper/UtilHelper.h"
#include "GameFramework/Character.h"
#include "AxeOfSword/SM/GAS/AOSAbilitySystemInitializeData.h"
#include "GameplayEffectTypes.h"
#include "BaseCharacter.generated.h"

class USphereComponent;
class UWeaponAnimation;
class UBaseAttribute;
class UEquipComponent;
class UAOSAbilitySystemComponent;

enum class ECharacterState : uint8;

UCLASS()
class AXEOFSWORD_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	GETTER_SETTER(ECharacterState, CurrentState)
	GETTER(TObjectPtr<UEquipComponent>, EquipComponent)
	GETTER(TObjectPtr<UBaseAttribute>, Attribute)
	GETTER(TObjectPtr<USphereComponent>, FistRightSphereCapsule)
	
	float GetHealth() const;

	void ToggleFistAttackMode(const bool Toggle);

protected:
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAOSAbilitySystemInitializeData> InitialData;

	UPROPERTY(EditDefaultsOnly)
	UAOSAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UEquipComponent> EquipComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> FistRightSphereCapsule;
	
	UPROPERTY()
	TObjectPtr<UBaseAttribute> Attribute;
	
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MovementSpeedChangedDelegateHandle;

	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMovementSpeedChanged(const FOnAttributeChangeData& Data);

private:
	ECharacterState CurrentState;
};
