#include "BaseAttribute.h"

void UBaseAttribute::Initialize()
{
	InitHealth(100);
	InitMaxHealth(100);
	InitAttack(5);
	InitMovementSpeed(280);
}

void UBaseAttribute::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (NewValue < 0) NewValue = 0;
}
