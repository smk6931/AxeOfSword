#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AxeOfSword/SM/Helper/UtilHelper.h"
#include "AT_PlayLevelSequence.generated.h"

class ULevelSequence;
class ALevelSequenceActor;
class ULevelSequencePlayer;

UCLASS()
class AXEOFSWORD_API UAT_PlayLevelSequence : public UAbilityTask
{
	GENERATED_BODY()

public:
	static UAT_PlayLevelSequence* InitialEvent(UGameplayAbility* Ability, ULevelSequence* LevelSequence);
	
	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;
	
	virtual void Activate() override;

private:
	UPROPERTY()
	ALevelSequenceActor* LevelSequenceActor;

	UPROPERTY()
	TObjectPtr<ULevelSequence> LevelSequence;
};
