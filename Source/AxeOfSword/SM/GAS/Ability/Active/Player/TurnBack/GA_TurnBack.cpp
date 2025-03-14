#include "GA_TurnBack.h"

#include "AbilitySystemComponent.h"
#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"
#include "AxeOfSword/SM/Helper/StateHelper.h"
#include "AxeOfSword/SM/Weapon/BaseWeapon.h"
#include "AxeOfSword/SM/Weapon/LeviathanAxe.h"

bool UGA_TurnBack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayTagContainer* SourceTags
	, const FGameplayTagContainer* TargetTags
	, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags
									, OptionalRelevantTags))
	{
		return false;
	}
	
	const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(BaseCharacter))
	{
		return false;
	}

	const ALeviathanAxe* LeviathanAxe = Cast<ALeviathanAxe>(BaseCharacter->GetEquipComponent()->GetMainWeapon());
	if (!LeviathanAxe)
	{
		return false;
	}

	if (!UStateHelper::IsIdle(GetAvatarActorFromActorInfo()))
	{
		return false;
	}

	if (LeviathanAxe->GetAxeStatus() == ELeviathanAxeState::Return)
	{
		return false;
	}
	
	// 현재 도끼가 가만히 있는 상태여야 하며, 동시에 현재 Owner가
	// BaseCharacter 가 아닌 즉 벽에 박혀있지 않는 상태인 경우 사용 가능하다.
	return LeviathanAxe->GetAxeStatus() == ELeviathanAxeState::Throw ||
		LeviathanAxe->GetAxeStatus() == ELeviathanAxeState::Thrown_Idle;
}

void UGA_TurnBack::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());

	if (!BaseCharacter)
	{
		return;
	}
	
	BaseCharacter->SetCurrentState(ECharacterState::WeaponTurnBack);
}

void UGA_TurnBack::ActivateAbility(const FGameplayAbilitySpecHandle Handle
                                   , const FGameplayAbilityActorInfo* ActorInfo
                                   , const FGameplayAbilityActivationInfo ActivationInfo
                                   , const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(BaseCharacter))
	{
		return;
	}

	const ALeviathanAxe* LeviathanAxe = Cast<ALeviathanAxe>(BaseCharacter->GetEquipComponent()->GetMainWeapon());
	if (!LeviathanAxe)
	{
		return;
	}

	BaseCharacter->GetEquipComponent()->ClearCombo();
	
	const bool IsThrowing = LeviathanAxe->GetAxeStatus() == ELeviathanAxeState::Throw;

	if (IsThrowing)
	{
		FGameplayTagContainer PlayerAbilityCancelTagContainer;
		PlayerAbilityCancelTagContainer.AddTag(AOSGameplayTags::Ability_Sprint);
		PlayerAbilityCancelTagContainer.AddTag(AOSGameplayTags::Ability_Attack_Throw);
		GetAbilitySystemComponentFromActorInfo()->CancelAbilities(&PlayerAbilityCancelTagContainer);

		FGameplayTagContainer WeaponSkillCancelTagContainer;
		WeaponSkillCancelTagContainer.AddTag(AOSGameplayTags::Skill_LeviathanAxe_Throw);
		WeaponSkillCancelTagContainer.AddTag(AOSGameplayTags::Skill_LeviathanAxe_ThrowAfterHit);
		LeviathanAxe->GetAbilitySystemComponent()->CancelAbilities(&WeaponSkillCancelTagContainer);

		GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(AOSGameplayTags::GameplayCue_Leviathan_TurnBack);

		return;
	}
	
	GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(AOSGameplayTags::GameplayCue_Leviathan_Shake);
}

void UGA_TurnBack::EndAbility(const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility
					, bWasCancelled);
}