// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AxeOfSword/Mk_Boss/EnemyFSM/EnemyFSM.h"
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
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Speed = 300.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* BossMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ASword> SwordFactory;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector Direction;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Hp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category=FSM)
	class UEnemyFSM* Fsm;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category=FSM)
	EEnemyState animState;

	UFUNCTION(BlueprintCallable)
	void DamageAnimation();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* DamageMontage;

	void DestroyBoss();

private:
	FTimerHandle TimerHandle;
};


