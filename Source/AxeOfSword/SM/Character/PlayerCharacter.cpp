#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AxeOfSword/SM/GAS/Attribute/BaseAttribute.h"
#include "AxeOfSword/SM/Player/AOSPlayerState.h"
#include "Component/PlayerCameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 350.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 50.f;
	GetCharacterMovement()->MaxWalkSpeed = 280;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->SetRelativeLocation({-60, -30, 140});
	SpringArm->SetRelativeRotation({0, 90, 0});
	SpringArm->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UPlayerCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArm);
}

void APlayerCharacter::BeginPlay()
{
	if (AAOSPlayerState* PS = GetPlayerState<AAOSPlayerState>())
	{
		AbilitySystemComponent = Cast<UAOSAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
		AbilitySystemComponent->Initialize(InitialData);
		Attribute = PS->GetAttribute();
		Attribute->Initialize();
	}
	
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
	Super::BeginPlay();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<
		UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered
		, this, &ThisClass::MoveTo);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered
																			, this, &ThisClass::Look);
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

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2d LookToValue = Value.Get<FVector2d>();
	AddControllerYawInput(LookToValue.X);
	AddControllerPitchInput(LookToValue.Y);
}