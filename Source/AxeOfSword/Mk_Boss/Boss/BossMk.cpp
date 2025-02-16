// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMk.h"

#include "AxeOfSword/Mk_Boss/EnemyFSM/EnemyFSM.h"
#include "AxeOfSword/Mk_Boss/Sword/Sword.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

class UBossHpWidget;
// Sets default values
ABossMk::ABossMk()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Hp = 150;

	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempBossMesh(
		TEXT("'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	if (TempBossMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(TempBossMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90),
			FRotator(0,-90,0));
	}
	
	ConstructorHelpers::FClassFinder<UAnimInstance> TempABP(TEXT("'/Game/Boss_MK/Animation/ABP_BossQuin.ABP_BossQuin_c'"));
	if (TempABP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TempABP.Class);
	}

	Fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("Fsm"));
}

// Called when the game starts or when spawned
void ABossMk::BeginPlay()
{
	Super::BeginPlay();

	FTransform SocketTransform = GetMesh()->GetSocketTransform(TEXT("hand_rSocket"), ERelativeTransformSpace::RTS_World);
	// Sword를 스폰함
	ASword* SpawnedSword = GetWorld()->SpawnActor<ASword>(SwordFactory, SocketTransform);
	UPrimitiveComponent* SwordRoot = Cast<UPrimitiveComponent>(SpawnedSword->GetRootComponent());
	if (SwordRoot != nullptr)
	{
		SwordRoot->IgnoreActorWhenMoving(this,true);
	}
	// SwordRoot->SetCollisionProfileName(TEXT("NoCollision"));  
	if (SpawnedSword)
	{
		// Sword를 손 소켓에 부착함
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		SpawnedSword->AttachToComponent(GetMesh(), AttachmentRules, TEXT("hand_rSocket"));
		// 부착 성공 여부를 디버그 메시지로 출력
	}
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
