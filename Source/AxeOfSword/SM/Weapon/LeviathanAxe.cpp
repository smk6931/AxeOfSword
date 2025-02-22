#include "LeviathanAxe.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/GAS/AOSAbilitySystemComponent.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"
#include "AxeOfSword/SM/Helper/StateHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ALeviathanAxe::ALeviathanAxe()
{
	TurnBackTimeline = CreateDefaultSubobject<UTimelineComponent>("TurnBack Timeline");
	
	WeaponPivot = CreateDefaultSubobject<USceneComponent>("Weapon Pivot");
	WeaponPivot->SetupAttachment(GetRootComponent());

	ConstructorHelpers::FObjectFinder<UAOSAbilitySystemInitializeData>
		TempInitialData(TEXT("/Script/AxeOfSword.AOSAbilitySystemInitializeData'"
					   "/Game/SM/Weapon/Cretos_Axe/DA_LeviathanAxe_InitialData.DA_LeviathanAxe_InitialData'"));

	if (TempInitialData.Succeeded())
	{
		InitialData = TempInitialData.Object;
	}
	
	
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

void ALeviathanAxe::Throw()
{
	CastWeaponSkill(AOSGameplayTags::Skill_LeviathanAxe_Throw);
}

void ALeviathanAxe::TurnBack(AActor* NewOwner)
{
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AOSGameplayTags::Skill_LeviathanAxe_Throw);
	AbilitySystemComponent->CancelAbilities(&TagContainer);
	
	if (AxeStatus == ELeviathanAxeStatus::Return)
	{
		return;
	}
	
	ABaseCharacter* Pawn = Cast<ABaseCharacter>(NewOwner);
	if (!IsValid(Pawn))
	{
		return;
	}

	// 처음에 방향과 회전 정보를 설정해준다.
	const FVector MoveTo = Pawn->GetMesh()->GetSocketLocation("WeaponSocket");
	const FRotator NewMoveToSet = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MoveTo);
	TurnBackStartLocation = GetActorLocation();
	
	SetAxeStatus(ELeviathanAxeStatus::Return);
	SetOwner(NewOwner);

	// Weapon 자체의 상대 좌표들을 전부 초기화해줌
	SetActorRotation(NewMoveToSet);
	WeaponMesh->SetRelativeLocation(FVector::ZeroVector);
	WeaponMesh->SetRelativeRotation(FRotator::ZeroRotator);
	
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

	DamageStack = FMath::Min<uint8>(DamageStack + 1, MaxDamageStack);
	
	OnHitDamage(OtherActor);
}

void ALeviathanAxe::OnHitDamage(AActor* TargetActor)
{
	UGameplayStatics::ApplyDamage(TargetActor,
		Damage, GetOwner()->GetInstigatorController(),
		GetOwner(), UDamageType::StaticClass());

	// 데미지에 따른 공격력 수치 조정
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(),
		FMath::Clamp(Damage / 250, 0.05, 0.1));

	float HitStopValue = static_cast<float>(Damage) / 10000 - DamageStack * 0.0001;
	
	// 주의사항: GlobalTimeDilation에 따라 Timeout의 시간도 늘어난다.
	GetWorld()->GetTimerManager().SetTimer(EndHitStopTimerHandle,
		FTimerDelegate::CreateUObject(this, &ThisClass::OnHitStopEnd),
		FMath::Clamp(HitStopValue
			, 0.001, 0.01), false);
}

void ALeviathanAxe::OnHitStopEnd()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
}

void ALeviathanAxe::OnTurnBackCallback(FVector Output)
{
	const ABaseCharacter* Pawn = Cast<ABaseCharacter>(GetOwner());
	const FVector MoveTo = Pawn->GetMesh()->GetSocketLocation("WeaponSocket");
	
	// 이동하면서 회전 값과 이동 방향 값을 설정해준다.
	const FVector NewMoveToSet = FMath::Lerp(TurnBackStartLocation, MoveTo, Output.X);
	const FRotator NewRotateToSet = UKismetMathLibrary::FindLookAtRotation(NewMoveToSet, MoveTo);
	SetActorLocation(NewMoveToSet);
	SetActorRotation(NewRotateToSet);
	
	// LocalOffset을 설정해줌으로써 살짝 오른쪽으로 회전할 수 있도록 처리한다.
	AddActorLocalOffset({0, Output.Y * TurnBackRightPower, 0});

	// 회전 방향을 설정해준다.
	const FRotator NewWeaponMeshRotate = FMath::Lerp(FRotator::ZeroRotator, FRotator(0, -90, 0), Output.X);
	WeaponMesh->SetRelativeRotation(NewWeaponMeshRotate);
	
	if (FVector::Distance(GetActorLocation(), MoveTo) <= 50)
	{
		OnTurnBackEnd();
		TurnBackTimeline->Stop();
	}
}

void ALeviathanAxe::OnTurnBackFinish()
{
	OnTurnBackEnd();
}

void ALeviathanAxe::OnTurnBackEnd()
{
	ABaseCharacter* Pawn = Cast<ABaseCharacter>(GetOwner());
	if (!IsValid(Pawn))
	{
		return;
	}

	UStateHelper::ClearState(Pawn->GetAbilitySystemComponent());
	SetAxeStatus(ELeviathanAxeStatus::Idle);
	WeaponMesh->SetRelativeTransform(InitialWeaponMeshTransform);
	AttachToComponent(Pawn->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");

	APlayerController* PC = Pawn->GetController<APlayerController>();
	if (!PC)
	{
		return;
	}
	PC->PlayerCameraManager->StartCameraShake(TurnBackEndCameraShake);
}
