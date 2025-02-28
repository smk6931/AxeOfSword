#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class ULegacyCameraShake;
class UPlayerCameraComponent;
class USpringArmComponent;
class UCameraComponent;
class UPlayerAttributeSet;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class AXEOFSWORD_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	GETTER(TObjectPtr<UPlayerCameraComponent>, CameraComponent)
	GETTER(TObjectPtr<USpringArmComponent>, SpringArm)
	GETTER(FVector, MoveDirection)
	GETTER_SETTER(TObjectPtr<APawn>, ExecutionTarget)

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void OnHealthChanged(const FOnAttributeChangeData& Data) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
	
private:
	FVector MoveDirection;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Options|Input"
		, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Options|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveInputAction;
	
	UPROPERTY(EditAnywhere, Category = "Options|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookInputAction;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerCameraComponent> CameraComponent;

	UFUNCTION()
	void MoveTo(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UPROPERTY()
	TObjectPtr<APawn> ExecutionTarget;

	void FindTarget();
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Camera", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ULegacyCameraShake> DefaultCameraShakeClass;
};
