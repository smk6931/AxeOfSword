#include "GC_LaunchNotify.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UGC_LaunchNotify::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType,
                                         const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(MyTarget, EventType, Parameters);

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MyTarget);
	if (!BaseCharacter)
	{
		return;
	}
	
	BaseCharacter->GetCharacterMovement()->Launch(LaunchTo);
}