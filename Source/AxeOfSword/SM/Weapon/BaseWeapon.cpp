// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "Components/BoxComponent.h"


ABaseWeapon::ABaseWeapon()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(RootComponent);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon Mesh");

	WeaponMesh->SetupAttachment(GetRootComponent());
	WeaponMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
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

void ABaseWeapon::OnOverlapWeaponCollision(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner() || OtherActor == this)
	{
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("하이1: %s"), *GetOwner()->GetName());
	UE_LOG(LogTemp, Display, TEXT("하이2: %s"), *OtherActor->GetName());
	UE_LOG(LogTemp, Display, TEXT("하이3: %s"), *this->GetName());
}

void ABaseWeapon::UpdateWeaponAttackable(const bool IsEnable)
{
	if (AttackCollision)
	{
		AttackCollision->SetCollisionEnabled(IsEnable ?
			ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	}
}
