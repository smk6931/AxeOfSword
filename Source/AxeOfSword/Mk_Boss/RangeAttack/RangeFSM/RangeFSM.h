// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RangeFSM.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXEOFSWORD_API URangeFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URangeFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	//스킬 컴포넌트 형성
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = FSM)
	class APlayerCharacter* Player;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ARangeMonster* RangeMonster;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class URadialForceComponent* RadialForce;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class AAIController* Ai;

public:// 스킬
	void AiMove();
	void ShockWave();
	void JumpMonster();
	void BackJump();

public:
	FVector PeDistance();

	FTimerHandle JumpTimer;
	FTimerHandle ShockTimer;
	FTimerHandle BackJumpTimer;
	
};
