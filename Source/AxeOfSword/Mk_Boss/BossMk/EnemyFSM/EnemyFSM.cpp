// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "AxeOfSword/Mk_Boss/BossMk/Boss/BossMk.h"
#include "AxeOfSword/Mk_Boss/BossMk/BossAnim/BossAnim.h"
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
		TripleAttack();
		break;
	case EEnemyState::Dash:
		Dash();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	}
}

void UEnemyFSM::IdleState()
{
	UE_LOG(LogTemp, Display, TEXT("IdleState"));
	// 첫 Idle 상태
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	// Idle 타임이 지나면 Move 상태로 전환
	
	if (CurrentTime > IdleDelayTime)
	{
		mState = EEnemyState::Move;
		CurrentTime = 0;
	}
}

void UEnemyFSM::MoveState()
{
	UE_LOG(LogTemp, Display, TEXT("MoveState"));
	//상대와 나의 방향 구하기
	FVector Direction = Player->GetActorLocation() - Boss->GetActorLocation();
	//적과 나의 거리를 구한다
	float Distance = Direction.Size();
	
	Direction.Normalize();
	// 정규화한다
	Boss->SetActorRotation(Direction.Rotation());
	//적에게 이동한다
	Boss->AddMovementInput(Direction);
	//적과의 거리가 공격 거리보다 작아졌을때
	if (AttackRange>Distance)
	{
		mState = EEnemyState::Attack;
	}
}

void UEnemyFSM::AttackState()
{
	UE_LOG(LogTemp, Display, TEXT("AttackState"));
	// 상대방으로 향하는 보스의 방향을 구한다
	FVector Direction = Player->GetActorLocation() - Boss->GetActorLocation();
	Direction.Normalize();

	//상대와 나의 거리를 구한다
	float Distance = FVector::Dist(Boss->GetActorLocation(), Player->GetActorLocation());
	
	// 거리가 공격 범위보다 커졌을때
	// 이동 상태로 전환한다

	// 공격범위가 상대와의 거리보다 작아졌을떄
	if (AttackRange < Distance)
	{
		// CPP와 FSM의 AnimState를 맞춘다
		mState = EEnemyState::Move;
	}
	else
	{
		mState = EEnemyState::JumpAttack;
	}
}

void UEnemyFSM::JumpAttack()
{
	UE_LOG(LogTemp, Display, TEXT("JumpAttack"));
	mState = Anim->animState;
}


void UEnemyFSM::RgAttack()
{
	UE_LOG(LogTemp, Display, TEXT("RgAttack"));
	mState = Anim->animState;
}

void UEnemyFSM::TripleAttack()
{
	UE_LOG(LogTemp, Display, TEXT("RgAttack"));
	mState = Anim->animState;
}

void UEnemyFSM::Dash()
{
	UE_LOG(LogTemp, Display, TEXT("Dash"));
	mState = Anim->animState;
}

