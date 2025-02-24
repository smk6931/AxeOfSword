// Fill out your copyright notice in the Description page of Project Settings.


#include "SpearMonster.h"

#include "AxeOfSword/Mk_Boss/SpearMonster/SpearAnim/SpearAnim.h"
#include "AxeOfSword/Mk_Boss/SpearMonster/SpearFSM/SpearFSM.h"


// Sets default values
ASpearMonster::ASpearMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalSpear = CreateDefaultSubobject<USkeletalMeshComponent>("Spear");
	SpearAnim = CreateDefaultSubobject<USpearAnim>("SpearAnim");
	SpearFSM = CreateDefaultSubobject<USpearFSM>("SpearFSM");

	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh(TEXT("'/Game/ParagonAurora/Characters/"
	"Heroes/Aurora/Skins/SpringCeremony/Meshes/Aurora_SpringCeremony.Aurora_SpringCeremony'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
	}

	ConstructorHelpers::FClassFinder<UAnimInstance>TempAnim(TEXT("'/Game/Boss_MK/SpearMonster/MySpearAnim.MySpearAnim_c'"));
	if (TempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TempAnim.Class);
	}
}

// Called when the game starts or when spawned
void ASpearMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpearMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASpearMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

