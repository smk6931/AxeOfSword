// Fill out your copyright notice in the Description page of Project Settings.


#include "HeavyAttackNotifyState.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"

void UHeavyAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(BaseCharacter))
	{
		return;
	}
	
	AOSGameplayTags::RemoveGameplayTag(BaseCharacter->GetAbilitySystemComponent(), AOSGameplayTags::Ability_Attack_Heavy, 0);
	AOSGameplayTags::RemoveGameplayTag(BaseCharacter->GetAbilitySystemComponent(), AOSGameplayTags::State_Attack, 0);
}
