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
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status, "Status", "Status 관련 최상위 트리 태그");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Combat, "Status.Combat", "현재 전투 모드로 들어갔음을 의미하는 태그");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue, "GameplayCue", "GameplayCue 관련 최상위 트리 태그");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Character_Camera_Move,
	"GameplayCue.Character.Camera.Move", "플레이어 카메라 무빙에 영향을 주는 태그");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Character_Camera_Move_Attack0,
	"GameplayCue.Character.Camera.Move.Attack0", "기본 공격 전용 Base가 되는 카메라 무빙");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Character_Camera_Move_Attack1,
	"GameplayCue.Character.Camera.Move.Attack1", "기본 공격 전용 1번에서 실행하는 카메라 무빙");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Character_Camera_Move_Attack2,
	"GameplayCue.Character.Camera.Move.Attack2", "기본 공격 전용 2번에서 실행하는 카메라 무빙");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Character_Camera_Move_Attack3,
	"GameplayCue.Character.Camera.Move.Attack3", "기본 공격 전용 3번에서 실행하는 카메라 무빙");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Character_Camera_Move_Attack4,
	"GameplayCue.Character.Camera.Move.Attack4", "기본 공격 전용 4번에서 실행하는 카메라 무빙");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Character_Camera_Move_HeavyAttack,
	"GameplayCue.Character.Camera.Move.HeavyAttack", "강공격 시 실행하는 카메라 무빙");
}