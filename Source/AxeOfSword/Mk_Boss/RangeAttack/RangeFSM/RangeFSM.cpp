// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeFSM.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "AxeOfSword/Mk_Boss/RangeAttack/RangeMonster/RangeMonster.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values for this component's properties
URangeFSM::URangeFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->Radius = 1000.f;
	RadialForce->ImpulseStrength = 500.f;
	RadialForce->bImpulseVelChange = true;
}

// Called when the game starts
void URangeFSM::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	RangeMonster = Cast<ARangeMonster>(GetOwner());
	RadialForce->SetupAttachment(RangeMonster->GetRootComponent());
	Ai = Cast<AAIController>(RangeMonster->GetController());
	
	GetWorld()->GetTimerManager().SetTimer(JumpTimer,this,&URangeFSM::JumpMonster,
		3.0f, false);
	
	GetWorld()->GetTimerManager().SetTimer(ShockTimer,this,&URangeFSM::ShockWave,
		5.0f, false);
}

// Called every frame
void URangeFSM::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)

{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AiMove();
}

// void URangeFSM::BackJump()
// {
// 	FVector dir = Player->GetActorLocation() - RangeMonster->GetActorLocation();
// 	dir.Normalize();
// 	RangeMonster->LaunchCharacter(dir,false,false);
// }

void URangeFSM::AiMove()
{
    FVector Destination = Player->GetActorLocation();
	FVector Direction = Destination - RangeMonster->GetActorLocation();
	float Distance = Direction.Size();

	Ai->MoveToLocation(Destination);
}

void URangeFSM::ShockWave()
{
	RadialForce->SetWorldLocation(RangeMonster->GetActorLocation());
	if (RadialForce)
	{
		RadialForce->FireImpulse();
		UE_LOG(LogTemp, Warning, TEXT("ShockWave"));
		DrawDebugSphere(GetWorld(),RadialForce->GetComponentLocation(), 1000, 16, FColor::Red, true, 1.0f);
		DrawDebugSphere(GetWorld(),RangeMonster->GetActorLocation(), 1000, 16, FColor::Red, true, 1.0f);
	}
}

void URangeFSM::JumpMonster()
{
	FVector dir = Player->GetActorLocation() - RangeMonster->GetActorLocation();
	dir.Normalize();
	FVector Power = Player->GetActorLocation() - RangeMonster->GetActorLocation() - dir*100;
	Power.Z = 500.f;
	RangeMonster->LaunchCharacter(Power,false,false);
}




