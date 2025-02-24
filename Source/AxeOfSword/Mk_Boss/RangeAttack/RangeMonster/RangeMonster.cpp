// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeMonster.h"
#include "AxeOfSword/Mk_Boss/RangeAttack/RangeAnim/RangeAnim.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"


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
	RangeAnim = Cast<URangeAnim>(GetMesh()->GetAnimInstance());
	// LeviathanAxe = Cast<ALeviathanAxe>(PlayerCharacter->)

	
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	if (CapsuleComp)
	{
		// 캡슐 컴포넌트의 위치를 가져옵니다.
		FVector CapsuleLocation = CapsuleComp->GetComponentLocation();

		// 캡슐 컴포넌트의 절반 높이를 가져옵니다.
		float HalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();

		// 중앙값 계산
		FVector CapsuleCenter = CapsuleLocation + FVector(0, 0, HalfHeight);

		// 중앙값 출력
		UE_LOG(LogTemp, Warning, TEXT("Capsule Center: %s"), *CapsuleCenter.ToString());
	}
    
}

float ARangeMonster::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	HP -= DamageAmount;
	RangeAnim -> PlayStuckDamage();
	
	UE_LOG(LogTemp, Display, TEXT("ARangeMonster::TakeDamage(),%f"), HP);

	
	KnockBackDestPos = GetActorLocation()-GetActorForwardVector()*20500;
	FVector from = GetActorLocation();
	float p = 3 * GetWorld()->GetDeltaSeconds();
	FVector updatePos = FMath::Lerp(from,KnockBackDestPos,p);
	SetActorLocation(KnockBackDestPos, true);

	// FRotator rotFrom = GetActorRotation();
	// FVector toward = -knockBackForce;
	// FRotator rotTo = UKismetMathLibrary::MakeRotFromZX(FVector::UpVector, toward);
	// FRotator destRot = FMath::Lerp(rotFrom,rotTo,p);
	// me->SetActorRotation(destRot);
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
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


