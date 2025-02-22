#include "AT_LeviathanAxe_Throw.h"

#include "AxeOfSword/SM/Weapon/LeviathanAxe.h"
#include "Kismet/KismetSystemLibrary.h"

UAT_LeviathanAxe_Throw* UAT_LeviathanAxe_Throw::InitialEvent(
	UGameplayAbility* OwningAbility, const FLeviathanAxeThrowBalance& NewBalance)
{
	UAT_LeviathanAxe_Throw* NewTask = NewAbilityTask<UAT_LeviathanAxe_Throw>(OwningAbility);
	NewTask->Balance = NewBalance;
	return NewTask;
}

void UAT_LeviathanAxe_Throw::Activate()
{
	Super::Activate();
	LeviathanAxe = Cast<ALeviathanAxe>(Ability->GetAvatarActorFromActorInfo());
	LeviathanAxe->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	if (const APawn* Pawn = Cast<APawn>(LeviathanAxe->GetOwner()))
	{
		ThrowRotate = Pawn->GetController()->GetControlRotation();
		LeviathanAxe->SetWeaponMeshRotation(Pawn->GetController()->GetControlRotation());
	}
	
	LeviathanAxe->SetAxeStatus(ELeviathanAxeStatus::Throw);
	GravityStack = 0;
	
	bTickingTask = true;
}

void UAT_LeviathanAxe_Throw::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	RotateByPowerInTick(DeltaTime);
	
	FVector ForwardVector = FRotationMatrix(ThrowRotate).GetUnitAxis(EAxis::X) * Balance.ThrowMovePower * DeltaTime;
	GravityStack += DeltaTime * DeltaTime;
	ForwardVector.Z -= Balance.GravityScale * GravityStack;

	LeviathanAxe->AddActorWorldOffset(ForwardVector);

	FHitResult HitResult;
	TraceWeaponThrow(HitResult);

	if (HitResult.bBlockingHit)
	{
		OnHitThrown(HitResult);
	}
}

void UAT_LeviathanAxe_Throw::TraceWeaponThrow(FHitResult& HitResult)
{
	const FVector PivotPoint = LeviathanAxe->GetWeaponPivot()->GetComponentLocation();
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(LeviathanAxe);
	
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), PivotPoint,
		PivotPoint, 50, TraceTypeQuery1, false, IgnoreActors,
		EDrawDebugTrace::None, HitResult, true);
}

void UAT_LeviathanAxe_Throw::OnHitThrown(const FHitResult& HitResult)
{
	GravityStack = 0;
	LeviathanAxe->SetAxeStatus(ELeviathanAxeStatus::Thrown_Idle);
	
	const FRotator ActorRotate = FRotationMatrix::MakeFromZY(HitResult.ImpactNormal,
		LeviathanAxe->GetActorRightVector()).Rotator();
	
	LeviathanAxe->SetActorRotation(ActorRotate);
	LeviathanAxe->SetActorLocation(HitResult.ImpactPoint);

	const FVector Location{-40, 0, 30};
	const FRotator Angle{-120, 0, 0};
	
	LeviathanAxe->GetWeaponMesh()->SetRelativeLocation(Location);
	LeviathanAxe->GetWeaponMesh()->SetRelativeRotation(Angle);
	
	LeviathanAxe->SetOwner(HitResult.GetActor());
}

void UAT_LeviathanAxe_Throw::RotateByPowerInTick(const float DeltaTime)
{
	UStaticMeshComponent* WeaponMesh = LeviathanAxe->GetWeaponMesh();
	
	const FRotator WeaponMeshRotation = WeaponMesh->GetRelativeRotation();
	const FQuat NewRotationQuat = FQuat(WeaponMeshRotation);
	const FQuat MoveToRotationQuat = FQuat(FVector(0, 1, 0), Balance.ThrowRotatePower * DeltaTime);
	FQuat NewMeshRotation = MoveToRotationQuat * NewRotationQuat;
	
	NewMeshRotation.X = 0;
	NewMeshRotation.Z = 0;
	
	WeaponMesh->SetRelativeRotation(NewMeshRotation.Rotator());
}