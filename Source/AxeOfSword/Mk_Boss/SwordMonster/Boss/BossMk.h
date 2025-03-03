// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/EnemyFSM/EnemyFSM.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* BossMesh;

	//보스 전용 칼
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ASword> SwordFactory;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Direction;
    //믄스터 Hp
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Hp = 300.f;
    //처형게이지 = Hp * 2 / 3
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExcutionGuage = Hp*2/3;
	//애니메이션 Fsm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FSM)
	UEnemyFSM* Fsm;

	//스폰된 보스칼을 담아두고 싶다
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ASword* BossSword;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBossAnim* BossAnim;
   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FSM)
	EEnemyState animState;

	//보스와 칼을 삭제
	void DestroyBoss();
	void DestroyBossSword();

	void DiedImmediately();
    // 죽음 애니메이션 호출
    UFUNCTION(BlueprintImplementableEvent)
	void PlayDieAnimation();
	// HpBarWidget 업데이트
	UFUNCTION(BlueprintImplementableEvent)
	void BlueTakeDamage();
	// 피격데미지 애니메이션 호출
	UFUNCTION(BlueprintImplementableEvent)
	void DamageAnimation();

private:
	FTimerHandle TimerHandle;
	FTimerHandle TimerHandleB;

	float AttackDamage;
};
