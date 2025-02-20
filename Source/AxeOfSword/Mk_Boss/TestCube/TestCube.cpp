// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCube.h"

#include "Components/BoxComponent.h"


// Sets default values
ATestCube::ATestCube()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	Root->SetGenerateOverlapEvents(true);
	Root->SetCollisionProfileName("TestCube");
}

// Called when the game starts or when spawned
void ATestCube::BeginPlay()
{
	Super::BeginPlay();
	Root->OnComponentBeginOverlap.AddDynamic(this, &ATestCube::OnMyBeginOverlap);
}

// Called every frame
void ATestCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestCube::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

