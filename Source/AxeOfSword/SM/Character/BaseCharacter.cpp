#include "BaseCharacter.h"

#include "AxeOfSword/SM/GAS/Attribute/BaseAttribute.h"
#include "Component/EquipComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseCharacter::ABaseCharacter()
{
	EquipComponent = CreateDefaultSubobject<UEquipComponent>("Equip Component");
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	HealthChangedDelegateHandle = AbilitySystemComponent->
		GetGameplayAttributeValueChangeDelegate(Attribute->GetHealthAttribute())
	.AddUObject(this, &ThisClass::OnHealthChanged);
	MovementSpeedChangedDelegateHandle = AbilitySystemComponent->
		GetGameplayAttributeValueChangeDelegate(Attribute->GetHealthAttribute())
	.AddUObject(this, &ThisClass::OnMovementSpeedChanged);
}

void ABaseCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	// TODO : 체력 0이 되면 처리할 내용 작성
}

void ABaseCharacter::OnMovementSpeedChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

float ABaseCharacter::GetHealth() const
{
	return Attribute->GetHealth();
}
