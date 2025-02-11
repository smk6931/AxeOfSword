#include "GameplayTagHelper.h"

#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"

namespace AOSGameplayTags
{
	void AddGameplayTag(UAbilitySystemComponent* ASC, const FGameplayTag Tag, const int32 Count, const bool bIsReplicated)
	{
		if (bIsReplicated)
		{
			ASC->SetReplicatedLooseGameplayTagCount(Tag, Count);
		}
		ASC->SetLooseGameplayTagCount(Tag, Count);
	}

	void RemoveGameplayTag(UAbilitySystemComponent* ASC, const FGameplayTag Tag, const int32 Count, const bool bIsReplicated)
	{
		if (bIsReplicated)
		{
			ASC->SetReplicatedLooseGameplayTagCount(Tag, Count);
		}
		ASC->SetLooseGameplayTagCount(Tag, Count);
	}

	void SetGameplayTag(UAbilitySystemComponent* ASC, const FGameplayTag Tag, const int32 Count, const bool bIsReplicated)
	{
		if (bIsReplicated)
		{
			ASC->SetReplicatedLooseGameplayTagCount(Tag, Count);
		}
		ASC->SetLooseGameplayTagCount(Tag, Count);
	}

	void SwapGameplayTag(UAbilitySystemComponent* ASC, const FGameplayTag FromTag, const FGameplayTag ToTag, const bool bIsReplicated)
	{
		const uint8 FromTagCount = ASC->GetGameplayTagCount(FromTag);
		RemoveGameplayTag(ASC, FromTag, FromTagCount, bIsReplicated);
		AddGameplayTag(ASC, ToTag, FromTagCount, bIsReplicated);
	}

	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability, "Ability", "Ability 관련 최상위 트리 태그");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Attack, "Ability.Attack", "공격 Ability 실행 상태를 의미함");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Attack_Default, "Ability.Attack.Default", "일반 공격 Ability 실행 상태를 의미함");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Attack_Heavy, "Ability.Attack.Heavy", "강공격 Ability 실행 상태를 의미함");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State, "State", "State 관련 최상위 트리 태그");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Attack, "State.Attack", "현재 공격 상태가 진행 중 임을 의미함");
}