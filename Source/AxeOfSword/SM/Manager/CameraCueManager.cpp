#include "CameraCueManager.h"

#include "AbilitySystemComponent.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"

void FCameraCueManager::CastAttackCameraShakeByPlayerAttack(UAbilitySystemComponent* ASC,
                                                            const APlayerCharacter* Player)
{
	if (!Player)
	{
		UE_LOG(LogTemp, Display, TEXT("하이?"))
	}
	
	if (ASC->HasMatchingGameplayTag(AOSGameplayTags::Ability_Attack_Heavy))
	{
		ASC->ExecuteGameplayCue(AOSGameplayTags::GameplayCue_Character_Camera_Shake_HeavyAttack);
		return;
	}

	const FString AttackShakeTagName = FString::Printf(TEXT("GameplayCue.Character.Camera.Shake.Attack%d"),
		Player->GetEquipComponent()->GetComboIndex() + 1);
	ASC->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag(FName(AttackShakeTagName)));
}
