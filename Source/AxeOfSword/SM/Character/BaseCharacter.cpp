#include "BaseCharacter.h"

#include "AxeOfSword/SM/GAS/Attribute/BaseAttribute.h"
#include "AxeOfSword/SM/Player/AOSPlayerState.h"
#include "Component/EquipComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseCharacter::ABaseCharacter()
{
	EquipComponent = CreateDefaultSubobject<UEquipComponent>("Equip Component");
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = Attribute->GetMovementSpeed();
	
	HealthChangedDelegateHandle = AbilitySystemComponent->
		GetGameplayAttributeValueChangeDelegate(Attribute->GetHealthAttribute())
	.AddUObject(this, &ThisClass::OnHealthChanged);
	MovementSpeedChangedDelegateHandle = AbilitySystemComponent->
		GetGameplayAttributeValueChangeDelegate(Attribute->GetMovementSpeedAttribute())
	.AddUObject(this, &ThisClass::OnMovementSpeedChanged);
}

float ABaseCharacter::TakeDamage(float DamageAmount
	, FDamageEvent const& DamageEvent, AController* EventInstigator
	, AActor* DamageCauser)
{
	AAOSPlayerState* PS = GetPlayerState<AAOSPlayerState>();
	if (!PS)
	{
		return 0;
	}
	PS->GetAttribute()->SetHealth(PS->GetAttribute()->GetHealth() - DamageAmount);
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator
							, DamageCauser);
}

void ABaseCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue == 0)
	{
		UE_LOG(LogTemp, Display, TEXT("으앙 듀금"))
		AbilitySystemComponent->CancelAllAbilities();
	}
	
	for (FGameplayAbilitySpec& ActivateAbility : AbilitySystemComponent->GetActivatableAbilities())
	{
		if (ActivateAbility.Ability->IsActive())
		{
			ActivateAbility.Ability->CancelAbility(ActivateAbility.Handle,
			ActivateAbility.Ability->GetCurrentActorInfo(), ActivateAbility.Ability->GetCurrentActivationInfo(), false);
		}
	}
}

void ABaseCharacter::OnMovementSpeedChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

float ABaseCharacter::GetHealth() const
{
	return Attribute->GetHealth();
}
