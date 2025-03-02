// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

#include "AxeOfSword/Mk_Boss/SwordMonster/Boss/BossMk.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/BossAnim/BossAnim.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/Sword/Sword.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"


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
	case EEnemyState::Attack:
		AttackState();
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
	}
}

void UEnemyFSM::IdleState()
{
	// 첫 Idle 상태
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	// Idle 타임이 지나면 Move 상태로 전환
	
	if (CurrentTime > IdleDelayTime)
	{
		mState = EEnemyState::Move;
		CurrentTime = 0;
		Anim->animState = mState;
	}
}

void UEnemyFSM::MoveState()
{
	// UE_LOG(LogTemp, Warning, TEXT("MoveState"));
	Boss->BossSword->SwordCapsule->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	//상대와 나의 방향 구하기
	FVector Direction = Player->GetActorLocation() - Boss->GetActorLocation();
	Direction.Z = 0.f;
	//적과 나의 거리를 구한다
	float Distance = Direction.Size();
	
	Direction.Normalize();
	Boss->SetActorRotation(Direction.Rotation());
	//적과의 거리가 공격범위보다 클 떄 적에게 이동한다
	if (Distance > AttackRange)
	{
		Boss->AddMovementInput(Direction);
		Walk = true;
		AttackStay = false;
	}
	//적과의 거리가 공격 거리보다 작아졌을때
	if (AttackRange>Distance)
	{
		// Boss->AttackStay();
		Walk = false;
		AttackStay = true;
		
		CurrentTime += GetWorld()->DeltaTimeSeconds;
		if (CurrentTime > AttackTime)
		{
			mState = EEnemyState::Attack;
			Anim->animState = mState;
			CurrentTime = 0;
		}
	}
}


void UEnemyFSM::AttackState()
{
	
	// UE_LOG(LogTemp, Warning, TEXT("AttackState"));
	// 상대방으로 향하는 보스의 방향을 구한다
	FVector Direction = Player->GetActorLocation() - Boss->GetActorLocation();
	Direction.Normalize();
	
	float Distance = FVector::Dist(Boss->GetActorLocation(), Player->GetActorLocation());
	
	// 거리가 공격 범위보다 커졌을때
	// 이동 상태로 전환한다
	if (AttackRange < Distance)
	{
		mState = EEnemyState::idle;
		Anim->animState = mState;
	}
	else
	{
		mState = EEnemyState::RgAttack;
		// Anim->animState = mState;
	}
}

void UEnemyFSM::RgAttack()
{
	
	// UE_LOG(LogTemp, Warning, TEXT("RgAttack"));
	mState = Anim->animState;
}

void UEnemyFSM::TripleAttack()
{
	
	// UE_LOG(LogTemp, Warning, TEXT("TrippleAttack"));
	mState = Anim->animState;
}

void UEnemyFSM::Dash()
{
	// UE_LOG(LogTemp, Warning, TEXT("DashState"));
	mState = Anim->animState;
}
