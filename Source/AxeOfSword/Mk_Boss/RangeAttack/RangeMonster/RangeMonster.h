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
	// (떄리는쪽)AppendDamage -> TakeDamage가 호출되고
								// -> TakeRadialDamage가 호출되고 Radial 관련된 처리 여기서 하고 반환하면
								// TakeDamage가 종료되면
								// TakeAnyDamage가 호출되고 마무리가 돼
								
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
						 class AController* EventInstigator, AActor* DamageCauser) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* RangeMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class URangeAnim* RangeAnim;

	// 플레이어 캐릭터 정보 가져오기
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class APlayerCharacter* PlayerCharacter;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	URangeFSM* RangeFSM;

	// 넉벡위치
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector KnockBackDestPos;
	
	// RangeMonster 체력
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float HP = 200.f;
	
	// 처형게이지
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ExecutionGauge = HP*2/3; 

	// 카메라 Shake
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Components")
	TSubclassOf<class UCameraShakeBase> cameraShake;

	// Dieㅏ이머
	FTimerHandle DeathTime;

public:
	// HpBar 업데이트
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHpbarWidget();

	// 몬스터사망
	void DestroySelf();
};
