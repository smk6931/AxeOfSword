#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class UPlayerSpringArmComponent;
class UPlayerCameraComponent;
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

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input"
		, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveInputAction;

	UFUNCTION()
	void MoveTo(const FInputActionValue& Value);
};
