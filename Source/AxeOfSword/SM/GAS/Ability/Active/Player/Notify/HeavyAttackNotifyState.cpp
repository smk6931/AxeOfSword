// Fill out your copyright notice in the Description page of Project Settings.


#include "HeavyAttackNotifyState.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"
#include "AxeOfSword/SM/Helper/StateHelper.h"

void UHeavyAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(BaseCharacter))
	{
		return;
	}

	UEquipComponent* EquipComponent = BaseCharacter->GetEquipComponent();
	AOSGameplayTags::SwapGameplayTag(BaseCharacter->GetAbilitySystemComponent(),
		AOSGameplayTags::State_Attack, AOSGameplayTags::State_Attack_Ing);
	EquipComponent->ToggleAttack(true);
}

void UHeavyAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp,
                                        UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(BaseCharacter))
	{
		return;
	}
	
	UStateHelper::ClearState(BaseCharacter->GetAbilitySystemComponent());
	AOSGameplayTags::RemoveGameplayTag(BaseCharacter->GetAbilitySystemComponent(),
	AOSGameplayTags::Ability_Attack_Heavy);
}
