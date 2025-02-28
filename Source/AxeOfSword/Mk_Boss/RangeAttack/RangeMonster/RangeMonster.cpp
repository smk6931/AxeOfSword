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
	if (ExecutionGauge > 0)
	{
		ExecutionGauge -= DamageAmount;
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(DeathTime,this,&ARangeMonster::DestroySelf,2,false);
	}
	if (HP > 0)
	{
		HP -= DamageAmount;
	}
	RangeAnim -> PlayStuckDamage();
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


// 오버랩 이벤트로 맞은 지점 넉백 확인
// if (OtherActor ) // 무기인지 확인
// {
// 	FVector HitLocation = SweepResult.ImpactPoint; // 충돌 지점
// 	FVector KnockbackDirection = (GetActorLocation() - HitLocation).GetSafeNormal();
// 	KnockbackDirection.Z = 0.0f; // 수직 방향 제거 (필요 시 수정 가능)
//
// 	float KnockbackStrength = 500.0f; // 넉백 강도
// 	FVector KnockbackVelocity = KnockbackDirection * KnockbackStrength;
//
// 	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
// 	{
// 		MoveComp->AddImpulse(KnockbackVelocity, true);
// 	}
// }
