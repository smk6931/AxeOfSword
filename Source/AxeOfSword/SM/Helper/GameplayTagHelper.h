#pragma once

#include "NativeGameplayTags.h"

struct FGameplayTag;
class UAbilitySystemComponent;

namespace AOSGameplayTags
{
	AXEOFSWORD_API void AddGameplayTag(UAbilitySystemComponent* ASC, const FGameplayTag Tag, const int32 Count, const bool bIsReplicated = false);
	AXEOFSWORD_API void RemoveGameplayTag(UAbilitySystemComponent* ASC, const FGameplayTag Tag, const int32 Count, const bool bIsReplicated = false);
	AXEOFSWORD_API void SetGameplayTag(UAbilitySystemComponent* ASC, const FGameplayTag Tag, const int32 Count, const bool bIsReplicated = false);
	AXEOFSWORD_API void SwapGameplayTag(UAbilitySystemComponent* ASC, const FGameplayTag FromTag, const FGameplayTag ToTag
		, const bool bIsReplicated = false);
	
	// Ability 관련 최상위 트리 태그로 여러개가 들어갈 수 있음
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack_Default)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack_Heavy)

	// 현재 플레이어의 상태 관련으로 단 하나만 들어갈 수 있는 Root 상태
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Attack)

	// 플레이어 상태 값을 넣는 곳으로 여러개가 들어갈 수 있는 부가적인 상태
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Combat)
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Hold)

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
	AXEOFSWORD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Character_Camera_Fov_ZoomOut)
}