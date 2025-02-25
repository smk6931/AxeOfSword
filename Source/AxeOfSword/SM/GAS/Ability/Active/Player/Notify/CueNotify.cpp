#include "CueNotify.h"

#include "AbilitySystemComponent.h"
#include "AxeOfSword/SM/Character/BaseCharacter.h"

void UCueNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                        const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (!IsValid(BaseCharacter))
	{
		return;
	}

	FGameplayCueParameters Param;
	Param.AbilityLevel = 1;
	Param.EffectCauser = BaseCharacter;
	
	BaseCharacter->GetAbilitySystemComponent()->ExecuteGameplayCue(CueTag, Param);
}
