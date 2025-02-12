// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMk.h"

#include "AxeOfSword/Mk_Boss/Sword/Sword.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ABossMk::ABossMk()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Hp = 100.f;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempBossMesh(
		TEXT("'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	
	if (TempBossMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(TempBossMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90),
			FRotator(0,-90,0));
	}
	
	ConstructorHelpers::FClassFinder<UAnimInstance> TempABP(TEXT("'/Game/Boss_MK/Animation/Abp_Boss.ABP_Boss_c'"));
	if (TempABP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TempABP.Class);
	}

	ConstructorHelpers::FClassFinder<UUserWidget> TempBossHpBar(TEXT("'/Game/Boss_MK/UI/WBP_BossHp.WBP_BossHp_c'"));
	if (TempBossHpBar.Succeeded())
	{
		BossWidgetFactory = TempBossHpBar.Class;
	}
}

// Called when the game starts or when spawned
void ABossMk::BeginPlay()
{
	Super::BeginPlay();

	FTransform SocketTransform = GetMesh()->GetSocketTransform(TEXT("hand_rSocket"), ERelativeTransformSpace::RTS_World);
	// Sword를 스폰함
	ASword* SpawnedSword = GetWorld()->SpawnActor<ASword>(SwordFactory, SocketTransform);
    
	if (SpawnedSword)
	{
		// Sword를 손 소켓에 부착함
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		SpawnedSword->AttachToComponent(GetMesh(), AttachmentRules, TEXT("hand_rSocket"));
		// 부착 성공 여부를 디버그 메시지로 출력
	}
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (BossWidgetFactory)
    {
    	if (PlayerController)
    	{
    		BossHpBar = CreateWidget<UUserWidget>(PlayerController, BossWidgetFactory);
    		BossHpBar->AddToViewport();
    	}
    }
}
	
	// if (BossWidgetFactory)
	// {
	// 	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	// 	if (PlayerController)
	// 	{
	// 		BossHpBar = CreateWidget<UUserWidget>(PlayerController, BossWidgetFactory);
	// 		if (BossHpBar)
	// 		{
	// 			BossHpBar->AddToViewport();
	// 		}
	// 	}
	// }


// Called every frame
void ABossMk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    Direction = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - this->GetActorLocation();
	Direction.Normalize();
	
	SetActorLocation(GetActorLocation() + DeltaTime * Speed * Direction);

	FRotator rot = UKismetMathLibrary::MakeRotFromXZ(Direction,GetActorUpVector());
	SetActorRotation(rot);

}

// Called to bind functionality to input
void ABossMk::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}



