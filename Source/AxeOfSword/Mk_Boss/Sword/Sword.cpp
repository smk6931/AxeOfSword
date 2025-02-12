// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"


// Sets default values
ASword::ASword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetupAttachment(Root);

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempSword(TEXT("'/Game/Fab/Orange_Plasma_Sword/orange_plasma_sword.orange_plasma_sword'"));
	if (tempSword.Object != NULL)
	{
		SwordMesh->SetStaticMesh(tempSword.Object);
		SwordMesh->SetRelativeLocation(FVector(10,-15,-140));
		SwordMesh->SetRelativeScale3D(FVector(0.4,0.4,0.4));
	}
}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

