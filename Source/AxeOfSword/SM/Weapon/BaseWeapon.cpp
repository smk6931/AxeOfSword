// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"


ABaseWeapon::ABaseWeapon()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(RootComponent);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon Mesh");

	WeaponMesh->SetupAttachment(GetRootComponent());
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseWeapon::EquipWeaponToTarget(USkeletalMeshComponent* TargetMesh)
{
	AttachToComponent(TargetMesh,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
}

