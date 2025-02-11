#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "LeviathanAxe.generated.h"

UCLASS()
class AXEOFSWORD_API ALeviathanAxe : public ABaseWeapon
{
	GENERATED_BODY()

public:
	ALeviathanAxe();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
