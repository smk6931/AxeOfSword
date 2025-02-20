#include "StateHelper.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagHelper.h"

bool UStateHelper::IsIdle(const UAbilitySystemComponent* ASC)
{
	return ASC->HasMatchingGameplayTag(AOSGameplayTags::State_Idle);
}

bool UStateHelper::IsDamaged(const UAbilitySystemComponent* ASC)
{
	return ASC->HasMatchingGameplayTag(AOSGameplayTags::State_Damaged);
}

bool UStateHelper::IsZoomIn(const UAbilitySystemComponent* ASC)
{
	return ASC->HasMatchingGameplayTag(AOSGameplayTags::Status_CloseHold);
}

bool UStateHelper::IsWaitForTurnBackWeapon(const UAbilitySystemComponent* ASC)
{
	return ASC->HasMatchingGameplayTag(AOSGameplayTags::State_TurnBack);
}

void UStateHelper::ClearState(UAbilitySystemComponent* ASC)
{
	FGameplayTagContainer OwnedTags;
	ASC->GetOwnedGameplayTags(OwnedTags);

	// 상위 태그로 시작하는 모든 하위 태그를 찾아서 제거
	for (const FGameplayTag& Tag : OwnedTags)
	{
		if (Tag.MatchesTag(AOSGameplayTags::State))
		{
			AOSGameplayTags::RemoveGameplayTag(ASC, Tag);
		}
	}

	AOSGameplayTags::SetGameplayTag(ASC, AOSGameplayTags::State_Idle, 1);
}
