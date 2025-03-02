#include "RangeFSM.h"
#include "AIController.h"
#include "AxeOfSword/Mk_Boss/RangeAttack/RangeAnim/RangeAnim.h"
#include "AxeOfSword/Mk_Boss/RangeAttack/RangeMonster/RangeMonster.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"


URangeFSM::URangeFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
}

// Called when the game starts
void URangeFSM::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	RangeMonster = Cast<ARangeMonster>(GetOwner());
	Ai = Cast<AAIController>(RangeMonster->GetController());
	Anim = Cast<URangeAnim>(RangeMonster->GetMesh()->GetAnimInstance());
	
	GetWorld()->GetTimerManager().SetTimer(StateSwitchTimer, this,&URangeFSM::SwitchState, 2.0f, true);
}

void URangeFSM::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)

{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SetActorRot();
	AiMoveToVector();
}

void URangeFSM::Avoid()
{
	UE_LOG(LogTemp, Warning, TEXT("RangeFSM::Avoid"));
	FVector dir = Player->GetActorLocation() - RangeMonster->GetActorLocation();
	dir.Normalize();
	FVector power = -dir * 750.f;
	power.Z = 250.f;
	RangeMonster->LaunchCharacter(power,false,false);
	Anim->PlayJumpBackFlip();
	mState = ERangeFSMState::Idle;
	Anim->animState = ERangeFSMState::Idle;
}

void URangeFSM::Idle()
{
	UE_LOG(LogTemp, Warning, TEXT("RangeFSM::Idle"));
	mState = ERangeFSMState::AiMove;
}

void URangeFSM::AiMove()
{
	UE_LOG(LogTemp, Warning, TEXT("RangeFSM::AIMove"));
	//플레이어 위치
    FVector Destination = Player->GetActorLocation();
	FVector Direction = Destination - RangeMonster->GetActorLocation();

	float RandomAngle = FMath::RandRange(0.0f,2.0f * PI);
	FVector Offset = FVector(FMath::Cos(RandomAngle) * 700, FMath::Sin(RandomAngle) * 700, 0);
	
	//플레이어 방향 벡터
	FVector Dir = Direction.GetSafeNormal();
	//플레이어와 나와의 거리
	FVector FinalDestiantion = Destination - Offset;
	
	// FVector dir = Player->GetActorLocation()-RangeMonster->GetActorLocation();
	// dir.Normalize();
	// Player(dir.Rotation());
	
	Ai->MoveToLocation(FinalDestiantion - Dir * 800);
	
	Anim->animState = ERangeFSMState::AiMove;
	
	mState = ERangeFSMState::JumpMonster;
}

void URangeFSM::ShockWave()
{
	UE_LOG(LogTemp, Warning, TEXT("RangeFSM::ShockWave"));
	FVector ImpulseOrigin = GetOwner()->GetActorLocation(); // 임펄스 발생 위치
	float ImpulseRadius = 500.0f; // 임펄스 반경
	float ImpulseStrength = 1000.0f;
	float DamageAmount = 20.f;
	RadialForce->SetWorldLocation(RangeMonster->GetActorLocation());
	RadialForce->Radius = ImpulseRadius;
	RadialForce->ImpulseStrength = ImpulseStrength;
	RadialForce->bImpulseVelChange = true;
	RadialForce->FireImpulse();
	
	auto pc = GetWorld()->GetFirstPlayerController();
	if (pc)
	{
		pc->PlayerCameraManager->StartCameraShake(RangeMonster->cameraShake);
	}
	
	UGameplayStatics::ApplyRadialDamage(GetOwner(), DamageAmount, ImpulseOrigin, ImpulseRadius,
		UDamageType::StaticClass(), OverlappingActors, GetOwner(), GetOwner()->GetInstigatorController(), true);

	// DrawDebugSphere(GetWorld(),RadialForce->GetComponentLocation(), 1000, 16, FColor::Red, true, 1.0f);
	// DrawDebugSphere(GetWorld(),RangeMonster->GetActorLocation(), 1000, 16, FColor::Red, true, 1.0f);
    mState = ERangeFSMState::Avoid;
}

void URangeFSM::JumpMonster()
{
	UE_LOG(LogTemp, Warning, TEXT("RangeFSM::JumpMonster"));
	FVector dir = Player->GetActorLocation() - RangeMonster->GetActorLocation();
	dir.Normalize();
	FVector Power = Player->GetActorLocation() - RangeMonster->GetActorLocation() - dir * 300;
	Power.Z = 550.f;
    
	Anim->PlayJumpAnimation();
	RangeMonster->LaunchCharacter(Power,false,false);
	mState = ERangeFSMState::ShockWave;
}

void URangeFSM::SetActorRot()
{
	FRotator Rotator = RangeMonster->GetActorRotation();
	FVector SetVec = Player->GetActorLocation() - RangeMonster->GetActorLocation(); 
	SetVec.Normalize();
	FRotator SetRot = SetVec.Rotation();
	RangeMonster->SetActorRotation(SetRot);
}

void URangeFSM::TimerDestroy()
{
	FTimerHandle DestroyTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle,
		this,&URangeFSM::Destroy,1.0f,false);
}

void URangeFSM::Destroy()
{
	UE_LOG(LogTemp, Warning, TEXT("RangeMonster::Destroy"));
	// RangeMonster->Destroy();
}


void URangeFSM::AiMoveToVector()
{
	//Velocity 구하기
	FVector Velocity = RangeMonster->GetVelocity();
	//전방벡터 구하기
	FVector ForwardVector = RangeMonster->GetActorForwardVector();
	//오른쪽벡터구하기
	FVector RightVector = RangeMonster->GetActorRightVector();
	//내적에서 전방, 측면 속도 구하기
	ForwardSpeed = FVector::DotProduct(Velocity, ForwardVector);
	RightSpeed = FVector::DotProduct(Velocity, RightVector);
	//전방, 측면속도 0 ~ 100 값 조절하기
	ForwardSpeed = FMath::Clamp(ForwardSpeed, -100.0f, 100.0f);
	RightSpeed = FMath::Clamp(RightSpeed, -100.0f, 100.0f);
	
	Anim->Vertical = ForwardSpeed;
	Anim->Horizontal = RightSpeed;
	// UE_LOG(LogTemp, Warning, TEXT("ForwardSpeed : %f, Velocity : %f,%f,%f"), Anim->Vertical, Velocity.X, Velocity.Y, Velocity.Z);
}


void URangeFSM::SwitchState()
{
	switch(mState)
	{
	case ERangeFSMState::Idle:
		Idle();
		break;
	case ERangeFSMState::AiMove:
		AiMove();
		break;
	case ERangeFSMState::JumpMonster:
		JumpMonster();
		break;
	case ERangeFSMState::ShockWave:
		ShockWave();
		break;
	case ERangeFSMState::Avoid:
		Avoid();
		break;
	}
}