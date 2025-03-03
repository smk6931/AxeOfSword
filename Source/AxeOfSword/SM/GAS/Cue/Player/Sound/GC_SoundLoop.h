#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "GC_SoundLoop.generated.h"

UCLASS()
class AXEOFSWORD_API AGC_SoundLoop : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AGC_SoundLoop();

protected:
	virtual void BeginPlay() override;
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundBase> SoundToPlay;

	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComponent;
};
