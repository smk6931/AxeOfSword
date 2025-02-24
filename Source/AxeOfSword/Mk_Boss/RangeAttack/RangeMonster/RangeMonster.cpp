// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeMonster.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ARangeMonster::ARangeMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RangeFSM = CreateDefaultSubobject<URangeFSM>(TEXT("RangeFSM"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("'/Game/ParagonAurora/"
	"Characters/Heroes/Aurora/Skins/GlacialEmpress/Meshes/Aurora_GlacialEmpress.Aurora_GlacialEmpress'"));
	if (tempMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -70),
			FRotator(0,-110,0));
	}
	ConstructorHelpers::FClassFinder<UAnimInstance>tempAnim(TEXT("'/Game/Boss_MK/RangeMonster/MyRangeAnim.MyRangeAnim_c'"));
	if (tempAnim.Class)
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}
}
// Called when the game starts or when spawned
void ARangeMonster::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
}
// Called every frame
void ARangeMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector dir = PlayerCharacter->GetActorLocation()-GetActorLocation();
	dir.Normalize();
	SetActorRotation(dir.Rotation());
}

// Called to bind functionality to input
void ARangeMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


