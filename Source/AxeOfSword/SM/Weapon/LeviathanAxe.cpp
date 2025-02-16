#include "LeviathanAxe.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ALeviathanAxe::ALeviathanAxe()
{
	AttackCollision = CreateDefaultSubobject<UBoxComponent>("Attack Collision");
	AttackCollision->SetupAttachment(WeaponMesh);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TurnBackTimeline = CreateDefaultSubobject<UTimelineComponent>("TurnBack Timeline");
	
	PrimaryActorTick.bCanEverTick = true;
}

void ALeviathanAxe::BeginPlay()
{
	Super::BeginPlay();
	AttackCollision->OnComponentBeginOverlap.AddDynamic(
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
	
	SetOwner(NewOwner);
	SetAxeStatus(ELeviathanAxeStatus::Return);
	TurnBackStartLocation = GetActorLocation();
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

	if (AxeStatus == ELeviathanAxeStatus::Throw)
	{
		OnHitThrown(OtherActor);
		return;
	}
	
	OnHitDamage(OtherActor);
}

void ALeviathanAxe::OnHitThrown(AActor* TargetActor)
{
	AxeStatus = ELeviathanAxeStatus::Idle;
	AttachToActor(TargetActor, FAttachmentTransformRules::KeepWorldTransform);
	SetOwner(TargetActor);
	SetActorRelativeRotation({-70,GetActorRotation().Yaw,GetActorRotation().Roll});
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
	if (const APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		SetActorRotation(Pawn->GetController()->GetControlRotation());
	}
	const FRotator MoveToRotator = GetActorRotation() * -1;
	const FVector ResultLocation = GetActorRightVector() * Output
			+ FRotationMatrix(MoveToRotator).GetUnitAxis(EAxis::X);
	
	UE_LOG(LogTemp, Display, TEXT("gkdl: %s"), *ResultLocation.ToString())
	
	AddActorLocalOffset(ResultLocation);
}

void ALeviathanAxe::OnTurnBackFinish()
{
}
