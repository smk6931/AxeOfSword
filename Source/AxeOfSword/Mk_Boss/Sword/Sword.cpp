// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"

#include "AxeOfSword/Mk_Boss/Boss/BossMk.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ASword::ASword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetupAttachment(Root);

	SworldCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sphere"));
	SworldCapsule->SetupAttachment(Root);

	SworldCapsule->SetRelativeTransform(FTransform(
	FRotator(0, 40, 0),
	FVector(-50, -5, 50),
	FVector(1.0f, 1.0f, 1.0f)));
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempSword(TEXT("'/Game/Fab/Orange_Plasma_Sword/orange_plasma_sword.orange_plasma_sword'"));
	if (tempSword.Object != NULL)
	{
		SwordMesh->SetStaticMesh(tempSword.Object);
		SwordMesh->SetRelativeLocation(FVector(10,-15,-140));
		SwordMesh->SetRelativeScale3D(FVector(0.4,0.4,0.4));
		SwordMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	SworldCapsule->SetGenerateOverlapEvents(true);
	SworldCapsule->SetCollisionProfileName("SworldCapsule");
}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();
	SworldCapsule->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnMyBeginOverlap);

	BossMk = Cast<ABossMk>(GetOwner());
}

// Called every frame
void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASword::OnMyBeginOverlap(UPrimitiveComponent* OverlappedCompnent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* PlayerPawn = Cast<AActor>(OtherActor);
	if (PlayerPawn)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
				TEXT("Boss Attack"));

			OtherActor->TakeDamage();
		}
	}
}

