// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"

#include "AxeOfSword/Mk_Boss/SwordMonster/Boss/BossMk.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASword::ASword()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetupAttachment(Root);

	SwordCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sphere"));
	SwordCapsule->SetupAttachment(Root);

	SwordCapsule->SetRelativeTransform(FTransform(
	FRotator(0, 40, 0),
	FVector(-50, -5, 50),
	FVector(1.0f, 1.0f, 1.0f)));
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempSword(TEXT("'/Game/Fab/Orange_Plasma_Sword/orange_plasma_sword.orange_plasma_sword'"));
	if (tempSword.Object != NULL)
	{
		SwordMesh->SetStaticMesh(tempSword.Object);
		SwordMesh->SetRelativeLocation(FVector(10,-15,-140));
		SwordMesh->SetRelativeScale3D(FVector(0.4,0.4,0.4));
		// 콜리전 비활성화한다
		SwordMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 검이 보이는것을 끈다
		SwordMesh->SetVisibility(false);
	}
	// 스워드 캡슐 콜리전을 비활성화 한다 - 공격했을시만 활성화 시키기 위해서
	SwordCapsule->SetGenerateOverlapEvents(false);
	SwordCapsule->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}	

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();
	// 웨폰캡슐의 OnComponent의 오버랩 이벤트가 실행되었을떄 OnMyBeginOverlap을 실행시킨다 
	SwordCapsule->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnMyBeginOverlap);
	BossMk = Cast<ABossMk>(GetOwner());
	SwordMesh->SetHiddenInGame(true);
}

void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASword::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != BossMk && OtherActor != GetOwner()) 
	{
		UE_LOG(LogTemp, Display, TEXT("DamageApplied: %s"), *OtherActor->GetName());
		UGameplayStatics::ApplyDamage(OtherActor, ApplyDamage, OtherActor->GetInstigatorController(),
		this, nullptr);
	}
}

