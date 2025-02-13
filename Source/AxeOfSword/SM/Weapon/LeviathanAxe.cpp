#include "LeviathanAxe.h"
#include "Components/BoxComponent.h"

ALeviathanAxe::ALeviathanAxe()
{
	AttackCollision = CreateDefaultSubobject<UBoxComponent>("Attack Collision");
	AttackCollision->SetupAttachment(WeaponMesh);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ALeviathanAxe::BeginPlay()
{
	Super::BeginPlay();
	AttackCollision->OnComponentBeginOverlap.AddDynamic(
		this, &ThisClass::OnOverlapWeaponCollision);
}

void ALeviathanAxe::OnOverlapWeaponCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapWeaponCollision(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	SweepResult);
	UE_LOG(LogTemp, Display, TEXT("하이1: %s"), *GetOwner()->GetName());
	UE_LOG(LogTemp, Display, TEXT("하이2: %s"), *OtherActor->GetName());
}