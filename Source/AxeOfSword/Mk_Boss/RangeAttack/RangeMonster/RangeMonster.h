// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AxeOfSword/Mk_Boss/RangeAttack/RangeFSM/RangeFSM.h"
#include "GameFramework/Character.h"
#include "RangeMonster.generated.h"

UCLASS()
class AXEOFSWORD_API ARangeMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARangeMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* RangeMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class URangeAnim* RangeAnim;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class APlayerCharacter* PlayerCharacter;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class ARangeStaff>RangeStaff;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	URangeFSM* RangeFSM;
};
