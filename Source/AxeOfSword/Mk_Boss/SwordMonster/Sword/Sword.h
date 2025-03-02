// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sword.generated.h"

UCLASS()
class AXEOFSWORD_API ASword : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USceneComponent* Root;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* SwordMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UCapsuleComponent* SwordCapsule;

	//스폰된 보스칼을 담아두고 싶다
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ABossMk* BossMk;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector VfxLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=VFX)
	UParticleSystem* SwordAttackVfx;

	float ApplyDamage = 10;

	FTimerHandle TimerHandle;
	
	UFUNCTION()
	void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	// UPROPERTY(EditAnywhere)
	// class USoundBase* SwordAttackSFX;

	// void AttackVfx();
};
