#include "BaseAttribute.h"

void UBaseAttribute::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (NewValue < 0) NewValue = 0;
}
