#include "AT_LeviathanAxe_ThrowAfterHit.h"

#include "AxeOfSword/SM/Weapon/LeviathanAxe.h"
#include "Kismet/KismetSystemLibrary.h"

UAT_LeviathanAxe_ThrowAfterHit* UAT_LeviathanAxe_ThrowAfterHit::InitialEvent(
	UGameplayAbility* OwningAbility, const FLeviathanAxeThrowAfterHitBalance& NewBalance)
{
	UAT_LeviathanAxe_ThrowAfterHit* NewTask = NewAbilityTask<UAT_LeviathanAxe_ThrowAfterHit>(OwningAbility);
	NewTask->Balance = NewBalance;
	NewTask->LeviathanAxe = Cast<ALeviathanAxe>(OwningAbility->GetAvatarActorFromActorInfo());
	return NewTask;
}

void UAT_LeviathanAxe_ThrowAfterHit::Activate()
{
	Super::Activate();
	
	LeviathanAxe->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	if (const APawn* Pawn = Cast<APawn>(LeviathanAxe->GetOwner()))
	{ 
		ThrowRotate = Pawn->GetController()->GetControlRotation();
		LeviathanAxe->SetWeaponMeshRotation(Pawn->GetController()->GetControlRotation());
	}

	LeviathanAxe->SetAxeStatus(ELeviathanAxeState::Throw);
	GravityStack = 0;
	
	bTickingTask = true;
}

void UAT_LeviathanAxe_ThrowAfterHit::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	RotateByPowerInTick(DeltaTime);

	FVector MoveToVector = FRotationMatrix(ThrowRotate).GetUnitAxis(EAxis::X).GetSafeNormal();
	MoveToVector.Z = 1;
	
	FVector ForwardVector = MoveToVector * Balance.ThrowMovePower * DeltaTime;
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

void UAT_LeviathanAxe_ThrowAfterHit::TraceWeaponThrow(FHitResult& HitResult)
{
	const FVector PivotPoint = LeviathanAxe->GetWeaponPivot()->GetComponentLocation();
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(LeviathanAxe);
	
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), PivotPoint,
		PivotPoint, 50, TraceTypeQuery1, false, IgnoreActors,
		EDrawDebugTrace::None, HitResult, true);
}

void UAT_LeviathanAxe_ThrowAfterHit::OnHitThrown(const FHitResult& HitResult)
{
	if (HitResult.GetActor()->IsA(APawn::StaticClass()))
	{
		return;
	}
	
	GravityStack = 0;
	LeviathanAxe->SetAxeStatus(ELeviathanAxeState::Thrown_Idle);
	
	const FRotator ActorRotate = FRotationMatrix::MakeFromZY(HitResult.ImpactNormal,
		LeviathanAxe->GetActorRightVector()).Rotator();
	
	LeviathanAxe->SetActorRotation(ActorRotate);
	LeviathanAxe->SetActorLocation(HitResult.ImpactPoint);

	// TODO: 해당 부분은 Leviathan Pivot Point를 가져오는 것으로 후에 처리할 것
	const FVector Location{-40, 0, 30};
	const FRotator Angle{-120, 0, 0};
	
	LeviathanAxe->GetWeaponMesh()->SetRelativeLocation(Location);
	LeviathanAxe->GetWeaponMesh()->SetRelativeRotation(Angle);
	
	LeviathanAxe->SetOwner(HitResult.GetActor());

	// 여기서 Ability를 강제로 종료시키게 처리한다.
	OnThrowAfterHitEndNotified.Broadcast();
}

void UAT_LeviathanAxe_ThrowAfterHit::RotateByPowerInTick(const float DeltaTime)
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