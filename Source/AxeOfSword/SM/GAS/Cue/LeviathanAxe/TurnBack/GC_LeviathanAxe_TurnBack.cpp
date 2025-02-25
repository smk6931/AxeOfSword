#include "GC_LeviathanAxe_TurnBack.h"

#include "AbilitySystemComponent.h"
#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"
#include "AxeOfSword/SM/Helper/StateHelper.h"
#include "AxeOfSword/SM/Weapon/LeviathanAxe.h"
#include "Kismet/KismetMathLibrary.h"

AGC_LeviathanAxe_TurnBack::AGC_LeviathanAxe_TurnBack()
{
	TurnBackTimeline = CreateDefaultSubobject<UTimelineComponent>("TurnBack Timeline");
}

void AGC_LeviathanAxe_TurnBack::BeginPlay()
{
	Super::BeginPlay();
	
	TurnBackCallback.BindDynamic(this, &ThisClass::OnTurnBackCallback);
	TurnBackFinish.BindDynamic(this, &ThisClass::OnTurnBackFinish);
	
	TurnBackTimeline->SetLooping(false);
	TurnBackTimeline->AddInterpVector(TurnBackTimingCurve, TurnBackCallback);
	TurnBackTimeline->SetTimelineFinishedFunc(TurnBackFinish);
}

bool AGC_LeviathanAxe_TurnBack::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (Super::OnExecute_Implementation(MyTarget, Parameters))
	{
		return true;
	}

	ABaseCharacter* BaseOwner = Cast<ABaseCharacter>(MyTarget);

	if (!BaseOwner)
	{
		return true;
	}

	TargetCharacter = BaseOwner;
	
	InitializeWeapon();

	TurnBackTimeline->PlayFromStart();
	
	return false;
}

void AGC_LeviathanAxe_TurnBack::InitializeWeapon()
{
	ALeviathanAxe* TargetWeapon = Cast<ALeviathanAxe>(TargetCharacter->GetEquipComponent()->GetMainWeapon());
	
	// 처음에 방향과 회전 정보를 설정해준다.
	const FVector MoveTo = TargetCharacter->GetMesh()->GetSocketLocation("WeaponSocket");
	const FRotator NewMoveToSet = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MoveTo);
	
	TurnBackStartLocation = TargetWeapon->GetActorLocation();
	
	TargetWeapon->SetAxeStatus(ELeviathanAxeState::Return);
	TargetWeapon->SetOwner(TargetCharacter);
	TargetWeapon->UpdateWeaponAttackable(false);

	// Weapon 자체의 상대 좌표들을 전부 초기화해줌
	TargetWeapon->SetActorRotation(NewMoveToSet);
	TargetWeapon->GetWeaponMesh()->SetRelativeLocation(FVector::ZeroVector);
	TargetWeapon->GetWeaponMesh()->SetRelativeRotation(FRotator::ZeroRotator);
}

void AGC_LeviathanAxe_TurnBack::OnTurnBackCallback(FVector Output)
{
	ALeviathanAxe* TargetWeapon = Cast<ALeviathanAxe>(TargetCharacter->GetEquipComponent()->GetMainWeapon());
	
	const ABaseCharacter* Pawn = Cast<ABaseCharacter>(TargetWeapon->GetOwner());
	if (!IsValid(Pawn))
	{
		return;
	}
	
	const FVector MoveTo = Pawn->GetMesh()->GetSocketLocation("WeaponSocket");
	
	// 이동하면서 회전 값과 이동 방향 값을 설정해준다.
	const FVector NewMoveToSet = FMath::Lerp(TurnBackStartLocation, MoveTo, Output.X);
	const FRotator NewRotateToSet = UKismetMathLibrary::FindLookAtRotation(NewMoveToSet, MoveTo);
	
	TargetWeapon->SetActorLocation(NewMoveToSet);
	TargetWeapon->SetActorRotation(NewRotateToSet);
	
	// LocalOffset을 설정해줌으로써 살짝 오른쪽으로 회전할 수 있도록 처리한다.
	TargetWeapon->AddActorLocalOffset({0, Output.Y * TurnBackRightPower, 0});

	// 회전 방향을 설정해준다.
	FRotator NewWeaponMeshRotate = FMath::Lerp(FRotator::ZeroRotator, FRotator(0, -90, 0), Output.X);
	NewWeaponMeshRotate.Pitch += Output.Z * 180;
	TargetWeapon->GetWeaponMesh()->SetRelativeRotation(NewWeaponMeshRotate);
	
	if (FVector::Distance(GetActorLocation(), MoveTo) <= 50)
	{
		OnTurnBackEnd();
		TurnBackTimeline->Stop();
	}
}

void AGC_LeviathanAxe_TurnBack::OnTurnBackFinish()
{
	OnTurnBackEnd();
}

void AGC_LeviathanAxe_TurnBack::OnTurnBackEnd() const
{
	ALeviathanAxe* TargetWeapon = Cast<ALeviathanAxe>(TargetCharacter->GetEquipComponent()->GetMainWeapon());
	
	UStateHelper::ClearState(TargetCharacter->GetAbilitySystemComponent());
	TargetWeapon->SetAxeStatus(ELeviathanAxeState::Idle);
	TargetWeapon->GetWeaponMesh()->SetRelativeTransform(TargetWeapon->GetInitialWeaponMeshTransform());
	TargetWeapon->AttachToComponent(TargetCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AOSGameplayTags::Ability_TurnBack);
	TargetCharacter->GetAbilitySystemComponent()->CancelAbilities(&TagContainer);
	
	const APlayerController* PC = TargetCharacter->GetController<APlayerController>();
	if (!PC)
	{
		return;
	}
	PC->PlayerCameraManager->StartCameraShake(TurnBackEndCameraShake);
}