// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeActor.h"

#include "Components/BoxComponent.h"


// Sets default values
ACubeActor::ACubeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Box->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(
		TEXT("'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMesh.Succeeded())
	{
		Box->SetStaticMesh(tempMesh.Object);
	}
}

// Called when the game starts or when spawned
void ACubeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

