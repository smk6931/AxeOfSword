// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeMonster.h"
#include "AxeOfSword/Mk_Boss/RangeAttack/RangeAnim/RangeAnim.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"


// Sets default values
ARangeMonster::ARangeMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RangeFSM = CreateDefaultSubobject<URangeFSM>(TEXT("RangeFSM"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("'/Game/Boss_MK/Monster_Range/RangeMonster/Range_Skin/"
	"Skins/GlacialEmpress/Meshes/Aurora_GlacialEmpress.Aurora_GlacialEmpress'"));
	if (tempMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -70),
			FRotator(0,-110,0));
	}
	ConstructorHelpers::FClassFinder<UAnimInstance>tempAnim(TEXT(
		"'/Game/Boss_MK/RangeMonster/MyRangeAnim.MyRangeAnim_c'"));
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
	RangeAnim = Cast<URangeAnim>(GetMesh()->GetAnimInstance());
}

float ARangeMonster::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	// if (ExecutionGauge > 0)
	// {
	// 	ExecutionGauge -= DamageAmount;
	// }
	if (HP > 0)
	{
		HP -= DamageAmount;
		if (HP < 1)
		{
			HP = 0;
			RangeAnim -> PlayStuckDamage();
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(RangeFSM->StateSwitchTimer);
		UE_LOG(LogTemp, Warning, TEXT("타이머가 정지되었습니다!"));
		
		UE_LOG(LogTemp,Warning,TEXT("RangeMonsterDie!!!"));
		RangeAnim->PlayDieAnimation();
		GetWorld()->GetTimerManager().SetTimer(DeathTime,this,&ARangeMonster::DestroySelf,2,false);
	}
	UE_LOG(LogTemp, Display, TEXT("ARangeMonster::TakeDamage(),%f"), HP);
	UpdateHpbarWidget();
	
	KnockBackDestPos = GetActorLocation()-GetActorForwardVector() * 40;
	FVector from = GetActorLocation();
	SetActorLocation(KnockBackDestPos);
	
	float p = 3 * GetWorld()->GetDeltaSeconds();
	FVector updatePos = FMath::Lerp(from,updatePos,p);
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

// Called every frame
void ARangeMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARangeMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ARangeMonster::DestroySelf()
{
	this->Destroy();
}