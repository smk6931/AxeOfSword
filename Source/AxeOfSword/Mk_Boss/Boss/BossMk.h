// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossMk.generated.h"

UCLASS()
class AXEOFSWORD_API ABossMk : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossMk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float CurrentSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* BossMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class AActor* aSword;

	

	
};
