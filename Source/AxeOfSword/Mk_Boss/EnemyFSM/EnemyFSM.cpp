// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "AxeOfSword/Mk_Boss/Boss/BossMk.h"
#include "AxeOfSword/Mk_Boss/BossAnim/BossAnim.h"
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

	Anim = Cast<UBossAnim>(Boss->GetMesh()->GetAnimInstance());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 현재 상태값 출력
	if (GEngine)
	{
		int32 BossHp = Boss->Hp;
		FString stateStr = UEnum::GetValueAsString(mState);
		GEngine->AddOnScreenDebugMessage(1, 0.0f, 
			FColor::Green,FString::Printf(
				TEXT("Boss Status: %s\n"
				"HP: %d"), *stateStr, BossHp));
	}

	
	switch (mState)
	{
	case EEnemyState::idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
		
	case EEnemyState::JumpAttack:
		JumpAttack();
		break;
	case EEnemyState::RgAttack:
		RgAttack();
		break;
	case EEnemyState::TrippleAttack:
		TrippleAttack();
		break;
	case EEnemyState::Dash:
		Dash();
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
		// Anim->animState=mState;
	}
}

void UEnemyFSM::MoveState()
{
	FVector Direction = Player->GetActorLocation() - Boss->GetActorLocation();
	float Distance = Direction.Size();
	Direction.Normalize();
	Boss->SetActorRotation(Direction.Rotation());
	if (AttackRange>Distance)
	{
		mState = EEnemyState::Attack;
		Anim->animState = mState;
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
}

void UEnemyFSM::JumpAttack()
{
	
}


void UEnemyFSM::RgAttack()
{
	
}

void UEnemyFSM::TrippleAttack()
{
	
}

void UEnemyFSM::Dash()
{
	FVector LaunchVector = Boss->GetActorRightVector() * 10000;
	Boss->LaunchCharacter(LaunchVector, false, false);
}

void UEnemyFSM::DamageState()
{
	
}

void UEnemyFSM::DieState()
{
	
}

