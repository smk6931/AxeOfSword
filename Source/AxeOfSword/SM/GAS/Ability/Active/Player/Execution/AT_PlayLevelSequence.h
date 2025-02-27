#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_PlayLevelSequence.generated.h"

class ULevelSequence;
class ALevelSequenceActor;
class ULevelSequencePlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCinematicEndNotify);

UCLASS()
class AXEOFSWORD_API UAT_PlayLevelSequence : public UAbilityTask
{
	GENERATED_BODY()

public:
	static UAT_PlayLevelSequence* InitialEvent(UGameplayAbility* Ability, ULevelSequence* LevelSequence);
	
	virtual void Activate() override;

	virtual void ExternalConfirm(bool bEndTask) override;
	
	FOnCinematicEndNotify OnCinematicEndNotify;

private:
	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;
	
	UPROPERTY()
	ALevelSequenceActor* LevelSequenceActor;

	UPROPERTY()
	TObjectPtr<ULevelSequence> LevelSequence;

	UFUNCTION()
	void OnFinish();
};
