#include "BaseWeapon.h"
#include "AxeOfSword/SM/GAS/AOSAbilitySystemComponent.h"

ABaseWeapon::ABaseWeapon()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	GizmoPoint = CreateDefaultSubobject<USceneComponent>("Gizmo Point");
	GizmoPoint->SetupAttachment(Root);
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon Mesh");
	WeaponMesh->SetupAttachment(GizmoPoint);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

UAbilitySystemComponent* ABaseWeapon::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	UpdateWeaponAttackable(false);
	AbilitySystemComponent->Initialize(InitialData);
}

void ABaseWeapon::EquipWeaponToTarget(USkeletalMeshComponent* TargetMesh)
{
	AttachToComponent(TargetMesh,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
}

void ABaseWeapon::OnOverlapWeaponCollision(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABaseWeapon::UpdateWeaponAttackable(const bool IsEnable)
{
	if (WeaponMesh)
	{
		WeaponMesh->SetCollisionEnabled(IsEnable ?
			ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	}
}

void ABaseWeapon::ClearDamageStack()
{
	DamageStack = 0;
}
