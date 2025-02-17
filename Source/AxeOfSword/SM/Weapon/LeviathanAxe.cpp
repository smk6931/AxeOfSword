#include "LeviathanAxe.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

ALeviathanAxe::ALeviathanAxe()
{
	TurnBackTimeline = CreateDefaultSubobject<UTimelineComponent>("TurnBack Timeline");
	
	WeaponStart = CreateDefaultSubobject<USceneComponent>("Weapon Start");
	WeaponStart->SetupAttachment(GetRootComponent());
	
	WeaponEnd = CreateDefaultSubobject<USceneComponent>("Weapon End");
	WeaponEnd->SetupAttachment(GetRootComponent());
	
	PrimaryActorTick.bCanEverTick = true;
}

void ALeviathanAxe::BeginPlay()
{
	Super::BeginPlay();
	WeaponMesh->OnComponentBeginOverlap.AddDynamic(
		this, &ThisClass::OnOverlapWeaponCollision);
	
	TurnBackCallback.BindDynamic(this, &ThisClass::OnTurnBackCallback);
	TurnBackFinish.BindDynamic(this, &ThisClass::OnTurnBackFinish);
	
	TurnBackTimeline->SetLooping(false);
	TurnBackTimeline->AddInterpFloat(TurnBackTimingCurve, TurnBackCallback);
	TurnBackTimeline->SetTimelineFinishedFunc(TurnBackFinish);
}

void ALeviathanAxe::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (AxeStatus)
	{
		case ELeviathanAxeStatus::Throw:
		{
			const FRotator WeaponMeshRotation = WeaponMesh->GetRelativeRotation();
			const FQuat NewRotationQuat = FQuat(WeaponMeshRotation);
			const FQuat MoveToRotationQuat = FQuat(FVector(0, 1, 0), ThrowRotatePower * DeltaSeconds);
			FQuat NewMeshRotation = MoveToRotationQuat * NewRotationQuat;
			NewMeshRotation.X = 0;
			NewMeshRotation.Z = 0;
			WeaponMesh->SetRelativeRotation(NewMeshRotation.Rotator());
	
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
		case ELeviathanAxeStatus::Return:
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
	
	ThrowRotate = Pawn->GetController()->GetControlRotation();
	
	SetAxeStatus(ELeviathanAxeStatus::Return);
	SetOwner(NewOwner);
	AttachToComponent(Pawn->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");

	// Weapon 자체의 상대 좌표들을 전부 초기화해줌
	SetActorRelativeLocation(FVector::ZeroVector);
	WeaponMesh->SetRelativeLocation(FVector::ZeroVector);
	WeaponMesh->SetRelativeRotation(FRotator::ZeroRotator);

	// 임시 지만 위의 처리가 종료된다면 Idle로 상태 변환
	// TODO: 원래는 Timeline or Tick 종료 시 실행되는 로직
	SetAxeStatus(ELeviathanAxeStatus::Idle);
	
	TurnBackStartLocation = GetActorLocation();
	// TurnBackTimeline->PlayFromStart();
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
	SetOwner(HitResult.GetActor());
	
	const FRotator ActorRotate = FRotationMatrix::MakeFromZY(HitResult.ImpactNormal,
		GetActorRightVector()).Rotator();
	
	SetActorRotation(ActorRotate);
	SetActorLocation(HitResult.ImpactPoint);

	const FVector Location{-40, 0, 30};
	const FRotator Angle{-120, 0, 0};
	
	WeaponMesh->SetRelativeLocation(Location);
	WeaponMesh->SetRelativeRotation(Angle);
}

void ALeviathanAxe::TraceWeaponThrow(FHitResult& HitResult)
{
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), WeaponStart->GetComponentLocation(),
		WeaponEnd->GetComponentLocation(), TraceTypeQuery1, false, IgnoreActors,
		EDrawDebugTrace::ForDuration, HitResult, true);
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

void ALeviathanAxe::OnTurnBackCallback(float Output)
{
	const FRotator MoveToRotator = ThrowRotate;
	const FVector ResultLocation = GetActorRightVector() * Output * 3
			+ MoveToRotator.Vector();
	
	UE_LOG(LogTemp, Display, TEXT("gkdl: %s"), *ResultLocation.ToString())
	
	SetActorLocation(GetActorLocation() + ResultLocation);
}

void ALeviathanAxe::OnTurnBackFinish()
{
}
