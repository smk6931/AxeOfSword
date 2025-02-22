#pragma once

#include "NativeGameplayTags.h"

struct FGameplayTag;
class UAbilitySystemComponent;

namespace AOSGameplayTags
{
	AXEOFSWORD_API void AddGameplayTag(UAbilitySystemComponent* ASC, const FGameplayTag Tag, const int32 Count = 1, const bool IsReplicated = false);
	AXEOFSWORD_API void RemoveGameplayTag(UAbilitySystemComponent* ASC, const FGameplayTag Tag, const int32 Count = -1, const bool IsReplicated = false);
	AXEOFSWORD_API void SetGameplayTag(UAbilitySystemComponent* ASC, const FGameplayTag Tag, const int32 Count, const bool IsReplicated = false);
	AXEOFSWORD_API void SwapGameplayTag(UAbilitySystemComponent* ASC, const FGameplayTag FromTag, const FGameplayTag ToTag
		, const bool IsReplicated = false);
	AXEOFSWORD_API bool HasGameplayTag(UAbilitySystemComponent* ASC, const FGameplayTag FindToTag);
	AXEOFSWORD_API void ToggleGameplayTag(UAbilitySystemComponent* ASC, const FGameplayTag Tag,
		const bool IsForceActive = false, const bool IsReplicated = false);
	
	// Ability 관련 최상위 트리 태그로 여러개가 들어갈 수 있음
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack_Default)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack_Heavy)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack_Throw)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_CloseHold)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Sprint)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_TurnBack)

	// 도끼 관련 능력
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Skill_LeviathanAxe_Throw)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Skill_LeviathanAxe_TurnBack)

	// Passive 관련 최상위 트리 태그로 여러개가 들어갈 수 있음
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Passive)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Passive_Damaged)

	// 현재 플레이어의 상태 관련으로 단 하나만 들어갈 수 있는 Root 상태
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Idle)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Attack)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Attack_Ing)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Casting)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Damaged)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_TurnBack)

	// 플레이어 상태 값을 넣는 곳으로 여러개가 들어갈 수 있는 부가적인 상태
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Combat)
	// TODO: 이름 변경 CloseHold는 잘못된 표현
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_CloseHold)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Attack_Hold)

	// GameplayCue 관련 태그
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Character_Camera_Move)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Character_Camera_Move_Attack0)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Character_Camera_Move_Attack1)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Character_Camera_Move_Attack2)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Character_Camera_Move_Attack3)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Character_Camera_Move_Attack4)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Character_Camera_Move_HeavyAttack)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Character_Camera_Fov)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Character_Camera_Fov_ZoomIn)
}