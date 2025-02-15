#include "GA_CloseHold.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "AxeOfSword/SM/Helper/StateHelper.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"

bool UGA_CloseHold::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return UStateHelper::IsIdle(GetAbilitySystemComponentFromActorInfo());
}

void UGA_CloseHold::PreActivate(const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate
	, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo
	, OnGameplayAbilityEndedDelegate, TriggerEventData);

	AOSGameplayTags::SwapGameplayTag(GetAbilitySystemComponentFromActorInfo(),
		AOSGameplayTags::State_Idle,AOSGameplayTags::State_CloseHold);
}

void UGA_CloseHold::ActivateAbility(const FGameplayAbilitySpecHandle Handle
									, const FGameplayAbilityActorInfo* ActorInfo
									, const FGameplayAbilityActivationInfo ActivationInfo
									, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(ActorInfo->AvatarActor);
	if (!IsValid(BaseCharacter))
	{
		return;
	}
	
	if (!IsValid(AT_AttackZoomAnim))
	{
		const UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
		AT_AttackZoomAnim = UPlayMontageWithEvent::InitialEvent(
			this, NAME_None,
			EquipComponent->GetZoomMontage(),
			FGameplayTagContainer()
		);
	} else
	{
		AT_AttackZoomAnim->EndTask();
	}
	
	AT_AttackZoomAnim->ReadyForActivation();
}

void UGA_CloseHold::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	FGameplayCueParameters Param;
	Param.AbilityLevel = 1;
	Param.EffectCauser = GetAvatarActorFromActorInfo();
	
	GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(
		AOSGameplayTags::GameplayCue_Character_Camera_Fov_ZoomOut, Param);

	UStateHelper::ClearState(GetAbilitySystemComponentFromActorInfo());
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_CloseHold::InputReleased(const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	UE_LOG(LogTemp, Display, TEXT("하이"))
	
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}