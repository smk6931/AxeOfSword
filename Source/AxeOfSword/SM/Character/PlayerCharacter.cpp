#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 350.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 50.f;
	GetCharacterMovement()->MaxWalkSpeed = 280;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (const APlayerController* PC = Cast<APlayerController>(
		GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<
		UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered
																			, this, &ThisClass::MoveTo);
	}
}

void APlayerCharacter::MoveTo(const FInputActionValue& Value)
{
	const FVector2d InputVector = Value.Get<FVector2d>();

	const FRotator MoveRotation = {0, GetController()->GetControlRotation().Yaw, 0};

	const FVector ForwardVector = FRotationMatrix(MoveRotation).GetUnitAxis(EAxis::X) * InputVector.X;
	const FVector RightVector = FRotationMatrix(MoveRotation).GetUnitAxis(EAxis::Y) * InputVector.Y;

	const FVector MoveTo = ForwardVector + RightVector;

	AddMovementInput(MoveTo, 1);
}