#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_CloseHold.generated.h"

UCLASS()
class AXEOFSWORD_API UAT_CloseHold : public UAbilityTask
{
	GENERATED_BODY()
public:
	static UAT_CloseHold* InitialEvent(UGameplayAbility* OwningAbility);

protected:
	virtual void TickTask(float DeltaTime) override;

	virtual void ExternalCancel() override;
};
