// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpearFSM.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXEOFSWORD_API USpearFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USpearFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ASpearMonster* SpearMonster;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class AAIController* AIController;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class APlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class ASpear> SpearFactory;

	FTimerHandle TimerHandle;

	// UPROPERTY(EditAnywhere,BlueprintReadWrite)
	// UNavigationSystemV1* NavigationSystem;

	void AiMove();
	// void ThrowSpear();
	
};
