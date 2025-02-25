#include "StateHelper.h"

#include "AbilitySystemComponent.h"
#include "EnumHelper.h"
#include "GameplayTagHelper.h"
#include "AxeOfSword/SM/Character/BaseCharacter.h"

bool UStateHelper::IsIdle(const AActor* Target)
{
	if (const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Target))
	{
		return BaseCharacter->GetCurrentState() == ECharacterState::Idle;
	}
	
	return false;
}

bool UStateHelper::IsDamaged(const AActor* Target)
{
	if (const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Target))
	{
		return BaseCharacter->GetCurrentState() == ECharacterState::Damaged;
	}
	
	return false;
}

bool UStateHelper::IsZoomIn(const UAbilitySystemComponent* ASC)
{
	return ASC->HasMatchingGameplayTag(AOSGameplayTags::Status_CloseHold);
}

bool UStateHelper::IsWaitForTurnBackWeapon(const AActor* Target)
{
	if (const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Target))
	{
		return BaseCharacter->GetCurrentState() == ECharacterState::WeaponTurnBack;
	}
	
	return false;
}

void UStateHelper::ClearState(AActor* Target)
{
	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Target))
	{
		BaseCharacter->SetCurrentState(ECharacterState::Idle);
	}
}
