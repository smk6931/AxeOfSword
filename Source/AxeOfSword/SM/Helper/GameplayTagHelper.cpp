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
		// Count가 -1 즉 기본 값으로 설정되어 있다면, 전부 없애기 위해 0으로 설정하고
		// 그게 아니라면 현재 갯수에서 N개만 빼게 설정한다.
		const int32 NewCount = Count == -1 ? 0 : ASC->GetGameplayTagCount(Tag) - Count;
		
		if (bIsReplicated)
		{
			ASC->SetReplicatedLooseGameplayTagCount(Tag, NewCount);
		}
		ASC->SetLooseGameplayTagCount(Tag, NewCount);
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
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Attack_Throw, "Ability.Attack.Throw", "강공격 Ability 실행 상태를 의미함");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_CloseHold, "Ability.CloseHold", "무기 조준 스킬을 사용하고 있음을 의미함");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Sprint, "Ability.Sprint", "달리기 Ability 사용하고 있음을 의미함");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_TurnBack, "Ability.TurnBack", "무기 복귀에 대한 Ability를 사용하고 있음을 의미함");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Passive, "Passive", "패시브 Ability 관련 최상위 트리 태그");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Passive_Damaged, "Passive.Damaged", "피격 시 발동하는 패시브 Ability 태그");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State, "State", "State 관련 최상위 트리 태그");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Idle, "State.Idle", "현재 아무 상태도 아닌 기본 상태임을 의미함");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Attack, "State.Attack", "현재 공격 상태가 진행 중 임을 의미함");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Casting, "State.Casting", "현재 스킬을 시전 중인 상태임을 의미함");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Damaged, "State.Damaged", "현재 피격 당한 상태임을 의미함");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_TurnBack, "State.TurnBack", "현재 무기 회수 단계에 돌입했음을 의미함");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status, "Status", "Status 관련 최상위 트리 태그");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Combat, "Status.Combat", "현재 전투 모드로 들어갔음을 의미하는 태그");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_CloseHold, "Status.CloseHold", "현재 조준하고 있음을 의미함");
	
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
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Character_Camera_Fov,
	"GameplayCue.Character.Camera.Fov", "카메라 줌 자체를 관리하기 위한 태그");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Character_Camera_Fov_ZoomIn,
	"GameplayCue.Character.Camera.Fov.ZoomIn", "카메라 줌 인 기능");
}