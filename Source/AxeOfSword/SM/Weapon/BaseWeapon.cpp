// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"


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

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	UpdateWeaponAttackable(false);
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