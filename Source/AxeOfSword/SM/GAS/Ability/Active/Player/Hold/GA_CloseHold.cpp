#include "GA_CloseHold.h"

#include "AbilitySystemComponent.h"
#include "AT_CloseHold.h"
#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "AxeOfSword/SM/UI/HUD/PlayerHUD.h"
#include "AxeOfSword/SM/UI/HUD/Module/ZoomWidget.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"
#include "AxeOfSword/SM/Helper/StateHelper.h"
#include "AxeOfSword/SM/Player/AOSPlayerController.h"

bool UGA_CloseHold::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
										const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
										const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return !GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(
		AOSGameplayTags::Status_Combat) && !UStateHelper::IsDamaged(GetAvatarActorFromActorInfo());
}

void UGA_CloseHold::PreActivate(const FGameplayAbilitySpecHandle Handle
                                , const FGameplayAbilityActorInfo* ActorInfo
                                , const FGameplayAbilityActivationInfo ActivationInfo
                                , FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate
                                , const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo
	, OnGameplayAbilityEndedDelegate, TriggerEventData);

	AOSGameplayTags::AddGameplayTag(GetAbilitySystemComponentFromActorInfo(),
		AOSGameplayTags::Status_CloseHold);
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

	const UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
	AT_AttackZoomAnim = UPlayMontageWithEvent::InitialEvent(
		this, NAME_None,
		EquipComponent->GetZoomMontage(),
		FGameplayTagContainer()
	);
	AT_AttackZoomAnim->ReadyForActivation();

	AT_CloseHold = UAT_CloseHold::InitialEvent(this);
	AT_CloseHold->ReadyForActivation();
}

void UGA_CloseHold::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	FGameplayCueParameters Param;
	Param.AbilityLevel = 2;
	Param.EffectCauser = GetAvatarActorFromActorInfo();
	
	GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(
		AOSGameplayTags::GameplayCue_Character_Camera_Fov_ZoomIn, Param);

	AOSGameplayTags::RemoveGameplayTag(GetAbilitySystemComponentFromActorInfo(),
		AOSGameplayTags::Status_CloseHold);
	
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo()))
	{
		const AAOSPlayerController* PC = Player->GetController<AAOSPlayerController>();
		PC->GetPlayerHUD()->GetZoomWidget()->ToggleZoomMode(false);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_CloseHold::InputReleased(const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
