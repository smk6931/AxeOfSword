// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "AxeOfSword/Mk_Boss/Boss/BossMk.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"


// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	Boss = Cast<ABossMk>(GetOwner());
	Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 현재 상태값 출력
	if (GEngine)
	{
		float BossHp = Boss->Hp;
		FString stateStr = UEnum::GetValueAsString(mState); 
		GEngine->AddOnScreenDebugMessage(1, 0.0f, 
			FColor::Green,FString::Printf(
				TEXT("Boss Status: %s\n"
				"HP: %f"), *stateStr, BossHp));
	}

	
	switch (mState)
	{
	case EEnemyState::idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
}


void UEnemyFSM::IdleState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	
	if (CurrentTime > IdleDelayTime)
	{
		mState = EEnemyState::Move;
		CurrentTime = 0;
	}
}

void UEnemyFSM::MoveState()
{
	FVector Direction = Player->GetActorLocation() - Boss->GetActorLocation();
	float Distance = Direction.Size();
	Direction.Normalize();

	Boss->AddMovementInput(Direction);
	
	if (AttackRange>Distance)
	{
		mState = EEnemyState::Attack;
	}
}

void UEnemyFSM::AttackState()
{
	FVector Direction = Player->GetActorLocation() - Boss->GetActorLocation();
	float Distance = FVector::Dist(Boss->GetActorLocation(), Player->GetActorLocation());
	if (AttackRange < Distance)
	{
		mState = EEnemyState::Move;
	}
	Direction.Normalize();
	
	Boss->SetActorRotation(Direction.Rotation());
}

void UEnemyFSM::RgAttackState()
{
	
}

void UEnemyFSM::DamageState()
{
}

void UEnemyFSM::DieState()
{
}

