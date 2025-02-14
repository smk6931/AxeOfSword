#include "BaseAttribute.h"

void UBaseAttribute::Initialize()
{
	InitHealth(20);
	InitMaxHealth(20);
	InitAttack(5);
	InitMovementSpeed(280);
}

void UBaseAttribute::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (NewValue < 0) NewValue = 0;
}
