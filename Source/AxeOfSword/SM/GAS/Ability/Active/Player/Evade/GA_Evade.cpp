#include "GA_Evade.h"

#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/Data/WeaponAnimation.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "AxeOfSword/SM/Weapon/BaseWeapon.h"

void UGA_Evade::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const APlayerCharacter* Player = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());

	if (!Player)
	{
		return;
	}
	
	EvadeAnim = UPlayMontageWithEvent::InitialEvent(this,
		NAME_None, Player->GetEquipComponent()->
		GetMainWeaponAnimationData()->GetEvadeAnim().FindRef(GetDirection()),
		FGameplayTagContainer());

	EvadeAnim->OnCompleted.AddDynamic(this, &ThisClass::OnEndEvade);
	
	UE_LOG(LogTemp, Display, TEXT("하이하이 : %p"), this);

	EvadeAnim->ReadyForActivation();
}

void UGA_Evade::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Evade::OnEndEvade(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

EMoveDirection UGA_Evade::GetDirection() const
{
	const APlayerCharacter* Player = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());

	if (!Player)
	{
		return EMoveDirection::Forward;
	}

	// TODO: 공통화될 때는 이 정보가 Parameter로 이전됨
	const FVector TargetDirection = Player->GetMoveDirection();

	if (TargetDirection.X == 1 && TargetDirection.Y == -1)
	{
		return EMoveDirection::ForwardLeft;
	}
	if (TargetDirection.X == 1 && TargetDirection.Y == 1)
	{
		return EMoveDirection::ForwardRight;
	}
	if (TargetDirection.X == -1 && TargetDirection.Y == -1)
	{
		return EMoveDirection::BackwardLeft;
	}
	if (TargetDirection.X == -1 && TargetDirection.Y == 0)
	{
		return EMoveDirection::Backward;
	}
	if (TargetDirection.X == -1 && TargetDirection.Y == 1)
	{
		return EMoveDirection::BackwardRight;
	}
	if (TargetDirection.X == 0 && TargetDirection.Y == -1)
	{
		return EMoveDirection::Left;
	}
	if (TargetDirection.X == 0 && TargetDirection.Y == 1)
	{
		return EMoveDirection::Right;
	}
	
	return EMoveDirection::Forward;
}
