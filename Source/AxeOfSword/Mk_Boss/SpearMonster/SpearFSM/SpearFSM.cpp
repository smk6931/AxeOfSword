// Fill out your copyright notice in the Description page of Project Settings.


#include "SpearFSM.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "AxeOfSword/Mk_Boss/SpearMonster/SpearActor/Spear.h"
#include "AxeOfSword/Mk_Boss/SpearMonster/SpearMonster/SpearMonster.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"


// Sets default values for this component's properties
USpearFSM::USpearFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void USpearFSM::BeginPlay()
{
	Super::BeginPlay();

	SpearMonster = Cast<ASpearMonster>(GetOwner());
	AIController = Cast<AAIController>(SpearMonster->GetController());
	PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&USpearFSM::AiMove,2.0f,true);
	
	// ...
}


// Called every frame
void USpearFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void USpearFSM::AiMove()
{
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	FVector RandomLocation;
	
		float RandomDistance = FMath::FRandRange(500.0f, 1000.0f);
		float RandomAngle = FMath::FRandRange(0.0f, 2.0f * PI);
		FVector Offset = FVector(FMath::Cos(RandomAngle) * RandomDistance, FMath::Sin(RandomAngle) * RandomDistance, 0);
		RandomLocation = PlayerLocation + Offset;
		AIController->MoveToLocation(RandomLocation);
		// ThrowSpear();
}

// void USpearFSM::ThrowSpear()
// {
// 	FTransform SocketTransform = SpearMonster()->GetMesh()->GetSocketTransform(TEXT("Muzzle_01"), ERelativeTransformSpace::RTS_World);
// 	GetWorld()->SpawnActor<ASpear>(SpearFactory,SocketTransform);
// }

