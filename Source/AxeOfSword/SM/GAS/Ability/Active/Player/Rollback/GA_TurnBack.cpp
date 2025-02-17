#include "GA_TurnBack.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
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

	// 현재 도끼가 가만히 있는 상태여야 하며, 동시에 현재 Owner가
	// BaseCharacter 가 아닌 즉 벽에 박혀있지 않는 상태인 경우 사용 가능하다.
	UE_LOG(LogTemp, Display, TEXT("Test: %s"), *LeviathanAxe->GetOwner()->GetName())
	return LeviathanAxe->GetAxeStatus() == ELeviathanAxeStatus::Throw ||
		LeviathanAxe->GetAxeStatus() == ELeviathanAxeStatus::Thrown_Idle;
}

void UGA_TurnBack::ActivateAbility(const FGameplayAbilitySpecHandle Handle
									, const FGameplayAbilityActorInfo* ActorInfo
									, const FGameplayAbilityActivationInfo ActivationInfo
									, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(BaseCharacter))
	{
		return;
	}

	ALeviathanAxe* LeviathanAxe = Cast<ALeviathanAxe>(BaseCharacter->GetEquipComponent()->GetMainWeapon());
	if (!LeviathanAxe)
	{
		return;
	}
	
	LeviathanAxe->TurnBack(BaseCharacter);

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UGA_TurnBack::EndAbility(const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility
					, bWasCancelled);
}