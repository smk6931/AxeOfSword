#include "GA_Attack.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "AxeOfSword/SM/Helper/StateHelper.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"
#include "AxeOfSword/SM/Weapon/BaseWeapon.h"
#include "AxeOfSword/SM/Weapon/LeviathanAxe.h"

bool UGA_Attack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
									FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// 이미 Ability_Attack 태그가 존재한다는 것은 해당 Ability가 실행 중이라는 것이기 때문에
	// Block 처리를 진행한다.
	return !IsAvatarDoingAttack();
}

void UGA_Attack::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	// 기본 상태가 아니라면 굳이 세팅할 필요가 없다.
	if (!GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(AOSGameplayTags::State_Idle))
	{
		return;
	}
	
	// Ability 실행 이전에는 현재 공격 Ability가 활성화 됨을 태그로 명시한다.
	AOSGameplayTags::SetGameplayTag(GetAbilitySystemComponentFromActorInfo(),
	AOSGameplayTags::Ability_Attack_Default, 1);
	AOSGameplayTags::SetGameplayTag(GetAbilitySystemComponentFromActorInfo(),
		AOSGameplayTags::Status_Combat, 1);
}

void UGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                 const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData* TriggerEventData)
{
	// 최초 실행
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(AOSGameplayTags::State_CloseHold))
	{
		IsHoldToThrow = true;
		return;
	}
	
	// 지금이 홀딩 상태임을 의미
	IsHoldEnd = false;
	
	// 기존의 콤보 초기화 관련 Timer는 초기화 시켜준다.
	DoComboAttack();
}

void UGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	// 던지기를 시도하기 위한 Press 인 경우에도 별도의 처리를 더 하지는 않는다.
	if (IsHoldToThrow)
	{
		return;
	}
	
	// 이미 공격 중인 상태 태그가 존재하는 경우 처리하면 안된다.
	if (IsAvatarDoingAttack())
	{
		return;
	}
	
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	// 일반 공격이 종료된 상태임에도 홀딩이 유지되고 있는 경우 강공격 전용 AnimMontage Task를
	// 실행시킴으로써 강공격을 계속해서 진행시킬 수 있도록 한다.
	if (!IsHoldEnd)
	{
		// 이미 공격 실행 상태이면 굳이 더 감지할 필요는 없다.
		if (IsAvatarDoingHeavyAttack())
		{
			return;
		}
		
		DoComboAttack();
	} else
	{
		// 홀딩이 끝난 경우에 대해서는 다시한번 Ability를 재실행 해준다.
		ActivateAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, &CurrentEventData);
	}
}

void UGA_Attack::InputReleased(const FGameplayAbilitySpecHandle Handle,
                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	// 던지기 상태라면 던지기 관련 로직만 실행시킨다.
	if (IsHoldToThrow)
	{
		DoThrowAttack();
		return;
	}
	
	IsHoldEnd = true;
	// 더이상 공격 상태가 아닌 경우: 공격 콤보가 끝난 경우가 이에 해당될 수 있다.
	if (!IsAvatarDoingAttack())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo,
			CurrentActivationInfo, false, false);
	}
}

void UGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo
	, bool bReplicateEndAbility, bool bWasCancelled)
{
	IsHoldToThrow = false;
	AOSGameplayTags::RemoveGameplayTag(GetAbilitySystemComponentFromActorInfo(),
		AOSGameplayTags::Status_Combat);
	AOSGameplayTags::RemoveGameplayTag(GetAbilitySystemComponentFromActorInfo(),
	AOSGameplayTags::Ability_Attack_Default);
	AOSGameplayTags::RemoveGameplayTag(GetAbilitySystemComponentFromActorInfo(),
	AOSGameplayTags::Ability_Attack_Heavy);
	AOSGameplayTags::RemoveGameplayTag(GetAbilitySystemComponentFromActorInfo(),
		AOSGameplayTags::Ability_Attack_Throw);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Attack::DoComboAttack()
{
	GetWorld()->GetTimerManager().ClearTimer(EndDefaultAttackHandle);

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(CurrentActorInfo->AvatarActor);
	if (!IsValid(BaseCharacter))
	{
		return;
	}
 
	UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
	
	AOSGameplayTags::SwapGameplayTag(GetAbilitySystemComponentFromActorInfo(),
		AOSGameplayTags::State_Idle, AOSGameplayTags::State_Attack);
	
	// 최초 실행 시 첫번째 Montage를 실행시킨다. 
	AT_ComboAttackAnim = UPlayMontageWithEvent::InitialEvent(
		this, NAME_None,
		EquipComponent->GetMainWeapon()->GetComboAttackAnim()[EquipComponent->GetComboIndex()],
		FGameplayTagContainer()
		);
	AT_ComboAttackAnim->OnCancelled.AddDynamic(this, &ThisClass::OnCancelAttack);
	AT_ComboAttackAnim->OnCompleted.AddDynamic(this, &ThisClass::OnEndAttack);
	AT_ComboAttackAnim->ReadyForActivation();
}

void UGA_Attack::DoHeavyAttack()
{
	AOSGameplayTags::RemoveGameplayTag(GetAbilitySystemComponentFromActorInfo(),
	AOSGameplayTags::Ability_Attack_Throw);
	AOSGameplayTags::RemoveGameplayTag(GetAbilitySystemComponentFromActorInfo(),
	AOSGameplayTags::Ability_Attack_Default);
	AOSGameplayTags::SetGameplayTag(GetAbilitySystemComponentFromActorInfo(),
	AOSGameplayTags::Ability_Attack_Heavy, 1);
	UStateHelper::ClearState(GetAbilitySystemComponentFromActorInfo());
	
	if (!IsValid(AT_HeavyAttackAnim))
	{
		const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(CurrentActorInfo->AvatarActor);
		if (!IsValid(BaseCharacter))
		{
			return;
		}
		
		const UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
		AT_HeavyAttackAnim = UPlayMontageWithEvent::InitialEvent(
			this, NAME_None,
			EquipComponent->GetMainWeapon()->GetHeavyAttackAnim(),
			FGameplayTagContainer()
			);
		AT_HeavyAttackAnim->OnCompleted.AddDynamic(this, &ThisClass::OnEndHeavyAttack);
	} else
	{
		AT_HeavyAttackAnim->EndTask();
	}
	
	AT_HeavyAttackAnim->ReadyForActivation();
}

void UGA_Attack::DoThrowAttack()
{
	AOSGameplayTags::RemoveGameplayTag(GetAbilitySystemComponentFromActorInfo(),
	AOSGameplayTags::Ability_Attack_Heavy);
	AOSGameplayTags::RemoveGameplayTag(GetAbilitySystemComponentFromActorInfo(),
	AOSGameplayTags::Ability_Attack_Default);
	AOSGameplayTags::SetGameplayTag(GetAbilitySystemComponentFromActorInfo(),
	AOSGameplayTags::Ability_Attack_Throw, 1);
	UStateHelper::ClearState(GetAbilitySystemComponentFromActorInfo());
	
	if (!IsValid(AT_ThrowAttackAnim))
	{
		const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(CurrentActorInfo->AvatarActor);
		if (!IsValid(BaseCharacter))
		{
			return;
		}
		
		const UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
		AT_ThrowAttackAnim = UPlayMontageWithEvent::InitialEvent(
			this, NAME_None,
			EquipComponent->GetMainWeapon()->GetThrowAttackAnim(),
			FGameplayTagContainer()
			);
		AT_ThrowAttackAnim->OnCompleted.AddDynamic(this, &ThisClass::OnEndThrowAttack);
	} else
	{
		AT_ThrowAttackAnim->EndTask();
	}
	
	AT_ThrowAttackAnim->ReadyForActivation();
}

void UGA_Attack::OnCancelAttack(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (IsHoldEnd)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo,
			CurrentActivationInfo, false, false);
	}
	GetWorld()->GetTimerManager().ClearTimer(EndDefaultAttackHandle);
}

void UGA_Attack::OnEndAttack(FGameplayTag EventTag, FGameplayEventData EventData)
{
	UStateHelper::ClearState(GetAbilitySystemComponentFromActorInfo());
	// 우선적으로 홀딩이 종료된 상태이면 애니메이션 끝난 뒤 자연스럽게 Ability도 종료시킨다.
	if (IsHoldEnd)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo,
			CurrentActivationInfo, false, false);
	}
	
	GetWorld()->GetTimerManager().ClearTimer(EndDefaultAttackHandle);
	GetWorld()->GetTimerManager().SetTimer(EndDefaultAttackHandle,
		FTimerDelegate::CreateUObject(this, &ThisClass::OnEndCombo), ComboEndDelayTime, false);
}

void UGA_Attack::OnEndHeavyAttack(FGameplayTag EventTag, FGameplayEventData EventData)
{
	AOSGameplayTags::RemoveGameplayTag(GetAbilitySystemComponentFromActorInfo(),
	AOSGameplayTags::Ability_Attack_Heavy);
	UStateHelper::ClearState(GetAbilitySystemComponentFromActorInfo());
}

void UGA_Attack::OnEndThrowAttack(FGameplayTag EventTag
	, FGameplayEventData EventData)
{
	AOSGameplayTags::RemoveGameplayTag(GetAbilitySystemComponentFromActorInfo(),
	AOSGameplayTags::Ability_Attack_Throw);
	UStateHelper::ClearState(GetAbilitySystemComponentFromActorInfo());
	EndAbility(CurrentSpecHandle, CurrentActorInfo,
		CurrentActivationInfo, false, false);
}

void UGA_Attack::OnEndCombo()
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(BaseCharacter))
	{
		return;
	}
	UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
	EquipComponent->ClearCombo();
}

bool UGA_Attack::IsAvatarDoingAttack() const
{
	return GetAbilitySystemComponentFromActorInfo()->
		HasMatchingGameplayTag(AOSGameplayTags::State_Attack);
}

bool UGA_Attack::IsAvatarDoingHeavyAttack() const
{
	return GetAbilitySystemComponentFromActorInfo()->
		HasMatchingGameplayTag(AOSGameplayTags::Ability_Attack_Heavy);
}