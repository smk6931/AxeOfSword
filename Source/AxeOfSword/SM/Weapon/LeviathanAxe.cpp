#include "LeviathanAxe.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"
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

	AttackTrailComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
		AttackTrailSystem, GetWeaponMesh()->GetSocketLocation("WeaponTrail"), GetWeaponMesh()->GetSocketRotation("WeaponTrail"));

	AttackTrailComponent->Activate();
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
