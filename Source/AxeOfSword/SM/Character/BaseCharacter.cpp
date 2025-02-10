#include "BaseCharacter.h"

#include "Component/EquipComponent.h"

ABaseCharacter::ABaseCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAOSAbilitySystemComponent>("Ability System Component");
	EquipComponent = CreateDefaultSubobject<UEquipComponent>("Equip Component");
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->Initialize();
}

