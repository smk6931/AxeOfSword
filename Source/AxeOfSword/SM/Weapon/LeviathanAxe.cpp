#include "LeviathanAxe.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ALeviathanAxe::ALeviathanAxe()
{
	TurnBackTimeline = CreateDefaultSubobject<UTimelineComponent>("TurnBack Timeline");
	
	WeaponPivot = CreateDefaultSubobject<USceneComponent>("Weapon Pivot");
	WeaponPivot->SetupAttachment(GetRootComponent());
	
	PrimaryActorTick.bCanEverTick = true;
}

void ALeviathanAxe::BeginPlay()
{
	Super::BeginPlay();

	InitialWeaponMeshTransform = WeaponMesh->GetRelativeTransform();
	
	WeaponMesh->OnComponentBeginOverlap.AddDynamic(
		this, &ThisClass::OnOverlapWeaponCollision);
	
	TurnBackCallback.BindDynamic(this, &ThisClass::OnTurnBackCallback);
	TurnBackFinish.BindDynamic(this, &ThisClass::OnTurnBackFinish);
	
	TurnBackTimeline->SetLooping(false);
	TurnBackTimeline->AddInterpVector(TurnBackTimingCurve, TurnBackCallback);
	TurnBackTimeline->SetTimelineFinishedFunc(TurnBackFinish);
}

void ALeviathanAxe::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (AxeStatus)
	{
		case ELeviathanAxeStatus::Return:
		{
			RotateByPowerInTick(DeltaSeconds);
			break;
		}
		case ELeviathanAxeStatus::Throw:
		{
			RotateByPowerInTick(DeltaSeconds);
	
			FVector ForwardVector = FRotationMatrix(ThrowRotate).GetUnitAxis(EAxis::X) * ThrowMovePower * DeltaSeconds;
			GravityStack += DeltaSeconds * DeltaSeconds;
			ForwardVector.Z -= GravityScale * GravityStack;

			AddActorWorldOffset(ForwardVector);

			FHitResult HitResult;
			TraceWeaponThrow(HitResult);

			if (HitResult.bBlockingHit)
			{
				OnHitThrown(HitResult);
			}
			break;
		}
		default:
		{
			break;
		}
	}
}

void ALeviathanAxe::Throw()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	if (const APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		ThrowRotate = Pawn->GetController()->GetControlRotation();
		WeaponMesh->SetRelativeRotation(Pawn->GetController()->GetControlRotation());
	}
	
	AxeStatus = ELeviathanAxeStatus::Throw;
}

void ALeviathanAxe::TurnBack(AActor* NewOwner)
{
	if (AxeStatus == ELeviathanAxeStatus::Return)
	{
		return;
	}
	
	ABaseCharacter* Pawn = Cast<ABaseCharacter>(NewOwner);
	if (!IsValid(Pawn))
	{
		return;
	}
	
	TurnBackStartLocation = GetActorLocation();
	
	SetAxeStatus(ELeviathanAxeStatus::Return);
	SetOwner(NewOwner);

	// Weapon 자체의 상대 좌표들을 전부 초기화해줌
	SetActorRelativeLocation(FVector::ZeroVector);
	SetActorRelativeRotation(FRotator::ZeroRotator);
	WeaponMesh->SetRelativeLocation(FVector::ZeroVector);
	WeaponMesh->SetRelativeRotation(FRotator::ZeroRotator);

	// 임시 지만 위의 처리가 종료된다면 Idle로 상태 변환
	// TODO: 원래는 Timeline or Tick 종료 시 실행되는 로직
	SetAxeStatus(ELeviathanAxeStatus::Idle);
	
	TurnBackTimeline->PlayFromStart();
}

void ALeviathanAxe::OnOverlapWeaponCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
											UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapWeaponCollision(OverlappedComponent, OtherActor,
		OtherComp, OtherBodyIndex, bFromSweep,SweepResult);
	
	// 무슨 상황이던간 Owner (본인 캐릭터)를 건드리는 일은 없다.
	if (OtherActor == GetOwner())
	{
		return;
	}
	
	OnHitDamage(OtherActor);
}

void ALeviathanAxe::OnHitThrown(const FHitResult& HitResult)
{
	GravityStack = 0;
	AxeStatus = ELeviathanAxeStatus::Thrown_Idle;
	
	const FRotator ActorRotate = FRotationMatrix::MakeFromZY(HitResult.ImpactNormal,
		GetActorRightVector()).Rotator();
	
	SetActorRotation(ActorRotate);
	SetActorLocation(HitResult.ImpactPoint);

	const FVector Location{-40, 0, 30};
	const FRotator Angle{-120, 0, 0};
	
	WeaponMesh->SetRelativeLocation(Location);
	WeaponMesh->SetRelativeRotation(Angle);
	
	SetOwner(HitResult.GetActor());
}

void ALeviathanAxe::TraceWeaponThrow(FHitResult& HitResult)
{
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), WeaponPivot->GetComponentLocation(),
		WeaponPivot->GetComponentLocation(), 50, TraceTypeQuery1, false, IgnoreActors,
		EDrawDebugTrace::None, HitResult, true);
}

void ALeviathanAxe::OnHitDamage(AActor* TargetActor)
{
	UGameplayStatics::ApplyDamage(TargetActor,
		Damage, GetOwner()->GetInstigatorController(),
		GetOwner(), UDamageType::StaticClass());

	// 데미지에 따른 공격력 수치 조정
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(),
		FMath::Clamp(Damage / 250, 0.05, 0.1));

	// 주의사항: GlobalTimeDilation에 따라 Timeout의 시간도 늘어난다.
	GetWorld()->GetTimerManager().SetTimer(EndHitStopTimerHandle,
		FTimerDelegate::CreateUObject(this, &ThisClass::OnHitStopEnd),
		FMath::Clamp(Damage / 10000, 0.005, 0.5), false);
}

void ALeviathanAxe::OnHitStopEnd()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
}

void ALeviathanAxe::OnTurnBackCallback(FVector Output)
{
	const FVector NewMoveToSet = FMath::Lerp(TurnBackStartLocation, GetOwner()->GetActorLocation(), Output.X);
	UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), NewMoveToSet);
	SetActorLocation(NewMoveToSet);
	AddActorLocalOffset({0, Output.Y * TurnBackRightPower, 0});
	ABaseCharacter* Pawn = Cast<ABaseCharacter>(GetOwner());
	
	if (!IsValid(Pawn))
	{
		return;
	}

	const FVector MoveTo = Pawn->GetMesh()->GetSocketLocation("WeaponSocket");
	
	if (FVector::Distance(GetActorLocation(), MoveTo) <= 50)
	{
		WeaponMesh->SetRelativeTransform(InitialWeaponMeshTransform);
		AttachToComponent(Pawn->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
		TurnBackTimeline->Stop();
	}
}

void ALeviathanAxe::OnTurnBackFinish()
{
	ABaseCharacter* Pawn = Cast<ABaseCharacter>(GetOwner());
	if (!IsValid(Pawn))
	{
		return;
	}
	
	WeaponMesh->SetRelativeTransform(InitialWeaponMeshTransform);
	AttachToComponent(Pawn->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
}

void ALeviathanAxe::RotateByPowerInTick(const float DeltaTime)
{
	const FRotator WeaponMeshRotation = WeaponMesh->GetRelativeRotation();
	const FQuat NewRotationQuat = FQuat(WeaponMeshRotation);
	const FQuat MoveToRotationQuat = FQuat(FVector(0, 1, 0), ThrowRotatePower * DeltaTime);
	FQuat NewMeshRotation = MoveToRotationQuat * NewRotationQuat;
	NewMeshRotation.X = 0;
	NewMeshRotation.Z = 0;
	WeaponMesh->SetRelativeRotation(NewMeshRotation.Rotator());
}