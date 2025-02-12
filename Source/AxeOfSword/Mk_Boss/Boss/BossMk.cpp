// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMk.h"


// Sets default values
ABossMk::ABossMk()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempBossMesh(
		TEXT("'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	
	if (TempBossMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(TempBossMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90),
			FRotator(0,-90,0));
	}
}

// Called when the game starts or when spawned
void ABossMk::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossMk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABossMk::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

