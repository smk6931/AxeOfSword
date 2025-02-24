// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeFSM.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "AxeOfSword/Mk_Boss/RangeAttack/RangeAnim/RangeAnim.h"
#include "AxeOfSword/Mk_Boss/RangeAttack/RangeMonster/RangeMonster.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values for this component's properties
URangeFSM::URangeFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
}

// Called when the game starts
void URangeFSM::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	RangeMonster = Cast<ARangeMonster>(GetOwner());
	RadialForce->SetupAttachment(RangeMonster->GetRootComponent());
	Ai = Cast<AAIController>(RangeMonster->GetController());

	Anim = Cast<URangeAnim>(RangeMonster->GetMesh()->GetAnimInstance());
	
	GetWorld()->GetTimerManager().SetTimer(StateSwitchTimer, this,&URangeFSM::SwitchState, 2.0f, true);
}

// Called every frame
void URangeFSM::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)

{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SetActorRot();
}

void URangeFSM::Avoid()
{
	UE_LOG(LogTemp, Warning, TEXT("RangeFSM::Avoid"));
	FVector dir = Player->GetActorLocation() - RangeMonster->GetActorLocation();
	dir.Normalize();
	FVector power = dir * 500.f;
	power.Z = -500.f;
	RangeMonster->LaunchCharacter(-power,false,false);
	Anim->PlayJumpBackFlip();
	mState = ERangeFSMState::Idle;
}

void URangeFSM::Idle()
{
	UE_LOG(LogTemp, Warning, TEXT("RangeFSM::Idle"));
	mState = ERangeFSMState::AiMove;
}

void URangeFSM::AiMove()
{
	UE_LOG(LogTemp, Warning, TEXT("RangeFSM::AIMove"));
	//플레이어 위치
    FVector Destination = RangeMonster->GetActorLocation();
	FVector Direction = Destination - RangeMonster->GetActorLocation();

	float RandomAngle = FMath::RandRange(0.0f,2.0f * PI);
	FVector Offset = FVector(FMath::Cos(RandomAngle) * 700, FMath::Sin(RandomAngle) * 700, 0);
	
	//플레이어 방향 벡터
	FVector Dir = Direction.GetSafeNormal();
	//플레이어와 나와의 거리
	FVector FinalDestiantion = Destination - Offset;
	
	Ai->MoveToLocation(FinalDestiantion - Dir * 300);
	
	mState = ERangeFSMState::JumpMonster;
}

void URangeFSM::ShockWave()
{
	UE_LOG(LogTemp, Warning, TEXT("RangeFSM::ShockWave"));
	FVector ImpulseOrigin = GetOwner()->GetActorLocation(); // 임펄스 발생 위치
	float ImpulseRadius = 500.0f; // 임펄스 반경
	float ImpulseStrength = 1000.0f;
	float DamageAmount = 20.f;
	RadialForce->SetWorldLocation(RangeMonster->GetActorLocation());
	RadialForce->Radius = ImpulseRadius;
	RadialForce->ImpulseStrength = ImpulseStrength;
	RadialForce->bImpulseVelChange = true;
	RadialForce->FireImpulse();
	
	UGameplayStatics::ApplyRadialDamage(GetOwner(), DamageAmount, ImpulseOrigin, ImpulseRadius,
		UDamageType::StaticClass(), OverlappingActors, GetOwner(), GetOwner()->GetInstigatorController(), true);

	DrawDebugSphere(GetWorld(),RadialForce->GetComponentLocation(), 1000, 16, FColor::Red, true, 1.0f);
	DrawDebugSphere(GetWorld(),RangeMonster->GetActorLocation(), 1000, 16, FColor::Red, true, 1.0f);
    mState = ERangeFSMState::Avoid;
}

void URangeFSM::JumpMonster()
{
	UE_LOG(LogTemp, Warning, TEXT("RangeFSM::JumpMonster"));
	FVector dir = Player->GetActorLocation() - RangeMonster->GetActorLocation();
	dir.Normalize();
	FVector Power = Player->GetActorLocation() - RangeMonster->GetActorLocation() - dir * 300;
	Power.Z = 700.f;
    
	Anim->PlayJumpAnimaition();
	RangeMonster->LaunchCharacter(Power,false,false);
	mState = ERangeFSMState::ShockWave;
}

void URangeFSM::SetActorRot()
{
	FRotator Rotator = RangeMonster->GetActorRotation();
	FVector SetVec = Player->GetActorLocation() - RangeMonster->GetActorLocation(); 
	SetVec.Normalize();
	FRotator SetRot = SetVec.Rotation();
	RangeMonster->SetActorRotation(SetRot);
}



void URangeFSM::SwitchState()
{
	switch(mState)
	{
	case ERangeFSMState::Idle:
		Idle();
		break;
	case ERangeFSMState::AiMove:
		AiMove();
		break;
	case ERangeFSMState::JumpMonster:
		JumpMonster();
		break;
	case ERangeFSMState::ShockWave:
		ShockWave();
		break;
	case ERangeFSMState::Avoid:
		Avoid();
		break;
	}
}



