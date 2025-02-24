// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpearMonster.generated.h"

UCLASS()
class AXEOFSWORD_API ASpearMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpearMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USpearAnim* SpearAnim;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* SkeletalSpear;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USpearFSM* SpearFSM;
};
