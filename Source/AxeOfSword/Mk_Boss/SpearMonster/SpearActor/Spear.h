// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/Sword/Sword.h"
#include "Spear.generated.h"

UCLASS()
class AXEOFSWORD_API ASpear : public ASword
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpear();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
