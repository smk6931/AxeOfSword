#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GC_SpawnNiagaraEffect.generated.h"

class UNiagaraSystem;

UCLASS()
class AXEOFSWORD_API UGC_SpawnNiagaraEffect : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

protected:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraSystem> Effect;
};
