#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_HeavyAttack.generated.h"

UCLASS()
class AXEOFSWORD_API UAT_HeavyAttack : public UAbilityTask
{
	GENERATED_BODY()

protected:
	virtual void InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent) override;
	
	virtual void TickTask(float DeltaTime) override;
};
