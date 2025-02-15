#include "LeviathanAxe.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ALeviathanAxe::ALeviathanAxe()
{
	AttackCollision = CreateDefaultSubobject<UBoxComponent>("Attack Collision");
	AttackCollision->SetupAttachment(WeaponMesh);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PrimaryActorTick.bCanEverTick = true;
}

void ALeviathanAxe::BeginPlay()
{
	Super::BeginPlay();
	AttackCollision->OnComponentBeginOverlap.AddDynamic(
		this, &ThisClass::OnOverlapWeaponCollision);
}

void ALeviathanAxe::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsThrow)
	{
		FRotator NewRotator = FRotator::ZeroRotator;
		NewRotator.Pitch += ThrowRotatePower * DeltaSeconds;
		
		const FVector ForwardVector = FRotationMatrix(ThrowRotate).GetUnitAxis(EAxis::X) * ThrowMovePower * DeltaSeconds;
	
		AddActorWorldOffset(ForwardVector);
		AddActorWorldRotation(NewRotator);
	}
}

void ALeviathanAxe::Throw(const FThrowParameter& ThrowParameter)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorRotation(ThrowParameter.ThrowRotate);
	IsThrow = true;

	ThrowMovePower = ThrowParameter.ThrowMovePower;
	ThrowRotatePower = ThrowParameter.ThrowRotatePower;
	ThrowRotate = ThrowParameter.ThrowRotate;
}

void ALeviathanAxe::OnOverlapWeaponCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
											UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapWeaponCollision(OverlappedComponent, OtherActor,
		OtherComp, OtherBodyIndex, bFromSweep,SweepResult);
	
	if (OtherActor == GetOwner())
	{
		return;
	}

	// const FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(
	// 	OtherActor->GetActorLocation(),
	// 	this->AttackCollision->GetComponentLocation());
	//
	// UE_LOG(LogTemp, Display, TEXT("테스트 데이터: %f, %f")
	// 	, Rotator.Yaw, Rotator.Pitch);

	UGameplayStatics::ApplyDamage(OtherActor,
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
