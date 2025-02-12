#include "BaseCharacter.h"

#include "Component/EquipComponent.h"

ABaseCharacter::ABaseCharacter()
{
	EquipComponent = CreateDefaultSubobject<UEquipComponent>("Equip Component");
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

