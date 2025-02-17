#include "LeviathanAxe.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

ALeviathanAxe::ALeviathanAxe()
{
	TurnBackTimeline = CreateDefaultSubobject<UTimelineComponent>("TurnBack Timeline");
	
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
			FRotator NewRotator = FRotator::ZeroRotator;
			NewRotator.Pitch += ThrowRotatePower * DeltaSeconds;
	
			FVector ForwardVector = FRotationMatrix(ThrowRotate).GetUnitAxis(EAxis::X) * ThrowMovePower * DeltaSeconds;
			GravityStack += DeltaSeconds * DeltaSeconds;
			ForwardVector.Z -= GravityScale * GravityStack;

			AddActorWorldOffset(ForwardVector);
			WeaponMesh->AddRelativeRotation(NewRotator);
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
	UpdateWeaponAttackable(true);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	if (const APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		ThrowRotate = Pawn->GetController()->GetControlRotation();
		SetActorRotation(Pawn->GetController()->GetControlRotation());
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

	if (AxeStatus == ELeviathanAxeStatus::Throw)
	{
		OnHitThrown(SweepResult);
		return;
	}
	
	OnHitDamage(OtherActor);
}

void ALeviathanAxe::OnHitThrown(const FHitResult& HitResult)
{
	GravityStack = 0;
	AxeStatus = ELeviathanAxeStatus::Thrown_Idle;
	UpdateWeaponAttackable(false);
	SetOwner(HitResult.GetActor());
	AttachToActor(HitResult.GetActor(), FAttachmentTransformRules::KeepWorldTransform);
	
	// 위치 초기화 (액터는 던진 방향을 기점으로 Rotate를 설정)
	SetActorRotation(ThrowRotate);
	FRotator NewRotator = HitResult.GetActor()->GetActorUpVector().Rotation();
	NewRotator.Yaw = 1;
	NewRotator.Roll = 1;
	NewRotator.Pitch -= 120;
	WeaponMesh->SetRelativeLocation(FVector(-45, 0, -22));
	WeaponMesh->SetRelativeRotation(NewRotator);

	const FVector AdjustLocation = GetActorLocation() - WeaponMesh->GetRelativeLocation();

	SetActorLocation(AdjustLocation);
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
