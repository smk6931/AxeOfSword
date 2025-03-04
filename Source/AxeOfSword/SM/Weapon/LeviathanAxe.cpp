#include "LeviathanAxe.h"

#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "AxeOfSword/SM/Character/Component/PlayerCameraComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

ALeviathanAxe::ALeviathanAxe()
{
	WeaponPivot = CreateDefaultSubobject<USceneComponent>("Weapon Pivot");
	WeaponPivot->SetupAttachment(GetRootComponent());

	ConstructorHelpers::FObjectFinder<UAOSAbilitySystemInitializeData>
		TempInitialData(TEXT("/Script/AxeOfSword.AOSAbilitySystemInitializeData'"
					   "/Game/SM/Weapon/Cretos_Axe/DA_LeviathanAxe_InitialData.DA_LeviathanAxe_InitialData'"));

	if (TempInitialData.Succeeded())
	{
		InitialData = TempInitialData.Object;
	}
}

void ALeviathanAxe::BeginPlay()
{
	Super::BeginPlay();
	
	WeaponMesh->OnComponentBeginOverlap.AddDynamic(
		this, &ThisClass::OnOverlapWeaponCollision);

	InitialWeaponMeshTransform = GetWeaponMesh()->GetRelativeTransform();
	
	AudioComponent = UGameplayStatics::SpawnSoundAttached(ThrowLoopSound, GetRootComponent());
	AudioComponent->Stop();

	AttackTrailFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(AttackTrailFXSystem,
		GetWeaponMesh(), FName("WeaponTrail"), FVector::ZeroVector, FRotator::ZeroRotator,
		 EAttachLocation::Type::KeepRelativeOffset, false);
	AttackTrailFXComponent->Deactivate();
}

void ALeviathanAxe::Throw()
{
	CastWeaponSkill(AOSGameplayTags::Skill_LeviathanAxe_Throw);
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

	SetCustomDilationInRange(300);
}

void ALeviathanAxe::SetCustomDilationInRange(const float Range)
{
	TArray<AActor*> IgnoreActor;
	
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetOwner()->GetActorLocation(),
		GetOwner()->GetActorLocation(), Range,TraceTypeQuery1,
		false, IgnoreActor, EDrawDebugTrace::None, HitStopHitResults, true);

	GetOwner()->CustomTimeDilation = FMath::Clamp(Damage / 250, 0.05, 0.1);
	
	for (FHitResult HitResult : HitStopHitResults)
	{
		HitResult.GetActor()->CustomTimeDilation = FMath::Clamp(Damage / 250, 0.05, 0.1);
		GetWorld()->GetTimerManager().SetTimer(EndHitStopTimerHandle,
		FTimerDelegate::CreateUObject(this, &ThisClass::OnHitStopEnd),
		0.05, false);
	}
}

void ALeviathanAxe::OnHitStopEnd()
{
	GetOwner()->CustomTimeDilation = 1;
	for (FHitResult HitStopHitResult : HitStopHitResults)
	{
		HitStopHitResult.GetActor()->CustomTimeDilation = 1;
	}
}

void ALeviathanAxe::SetPlayThrowSound(const bool IsEnable) const
{
	if (!AudioComponent)
	{
		return;
	}
	
	if (IsEnable)
	{
		AudioComponent->Play();
	} else
	{
		AudioComponent->Stop();
	}
}

void ALeviathanAxe::SetPlayEffect(const bool IsEnable) const
{
	if (IsEnable)
	{
		AttackTrailFXComponent->Activate();
	}
	else
	{
		AttackTrailFXComponent->Deactivate();
	}
}
