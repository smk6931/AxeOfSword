#include "BossMk.h"
#include "TimerManager.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/BossAnim/BossAnim.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/Sword/Sword.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"

class UBossHpWidget;
// Sets default values
ABossMk::ABossMk()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempBossMesh(
		TEXT("'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	if (TempBossMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(TempBossMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90),
			FRotator(0,-90,0));
	}
	// ConstructorHelpers::FClassFinder<UAnimInstance> TempAnim(TEXT("'/Game/Boss_MK/Animation/ABP_BossQuin.ABP_BossQuin'"));
	// if (TempAnim.Succeeded())
	// {
	// 	GetMesh()->SetAnimInstanceClass(TempAnim.Class);
	// }
	// AttackDamage = 20.f;
}

// Called when the game starts or when spawned
void ABossMk::BeginPlay()
{
	Super::BeginPlay();

	Hp = 200.f;
	UE_LOG(LogTemp, Display, TEXT("Hp%f"),Hp);

	BossAnim = Cast<UBossAnim>(GetMesh()->GetAnimInstance());

	FTransform SocketTransform = GetMesh()->GetSocketTransform(TEXT("hand_rSocket"), ERelativeTransformSpace::RTS_World);
	// Sword를 스폰함
	// 스폰된 보스칼을 담아두고 싶다
	BossSword = GetWorld()->SpawnActor<ASword>(SwordFactory, SocketTransform);
	UPrimitiveComponent* SwordRoot = Cast<UPrimitiveComponent>(BossSword->GetRootComponent());
	if (SwordRoot != nullptr)
	{
		SwordRoot->IgnoreActorWhenMoving(this,true);
	}
	if (BossSword)
	{
		// Sword를 손 소켓에 부착함
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		BossSword->AttachToComponent(GetMesh(), AttachmentRules, TEXT("hand_rSocket"));
		// 부착 성공 여부를 디버그 메시지로 출력
	}
}

// Called every frame
void ABossMk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// if (BossSword->SwordCapsule->IsCollisionEnabled())
	// {
	// 	UE_LOG(LogTemp, Display, TEXT("Capsule Collision is Enabled"));
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Display, TEXT("Capsule Collision is Disabled"));
	// }
}
// Called to bind functionality to input
void ABossMk::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
//
// void ABossMk::DamageAnimation()
// {
// 	if (DamageMontage && GetMesh())
// 	{
// 		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
// 		if (AnimInstance)
// 		{
// 			AnimInstance->Montage_Play(DamageMontage);
// 		}
// 	}
// }

void ABossMk::DiedImmediately()
{
	DestroyBossSword();
	DestroyBoss();
}

void ABossMk::DestroyBoss()
{
	Destroy();
}

void ABossMk::DestroyBossSword()
{
	BossSword->Destroy();
}

float ABossMk::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                          class AController* EventInstigator, AActor* DamageCauser)
{
	Hp -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("BossMk::HP%f"),Hp);
	BossAnim->animState = EEnemyState::idle;
	
	if (Hp < 1)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ABossMk::DestroyBossSword, 0.5f, true);
		// 1초 뒤에 DestroyBoss 함수 호출
		GetWorldTimerManager().SetTimer(TimerHandleB, this, &ABossMk::DestroyBoss, 1.0f, true);
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

