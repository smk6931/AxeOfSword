#include "BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAOSAbilitySystemComponent>("Ability System Component");
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->Initialize();
}

