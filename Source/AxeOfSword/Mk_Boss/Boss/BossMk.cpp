#include "BossMk.h"
#include "TimerManager.h"
#include "AxeOfSword/Mk_Boss/Sword/Sword.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

class UBossHpWidget;
// Sets default values
ABossMk::ABossMk()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Hp = 100;

	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempBossMesh(
		TEXT("'/Game/ParagonAurora/Characters/Heroes/Aurora/Skins/GlacialEmpress/Meshes/Aurora_GlacialEmpress.Aurora_GlacialEmpress'"));
	if (TempBossMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(TempBossMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90),
			FRotator(0,-90,0));
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> TempAnim(TEXT("'/Game/ParagonAurora/Characters/Heroes/Aurora/Aurora_AnimBlueprint.Aurora_AnimBlueprint_c'"));
	if (TempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TempAnim.Class);
	}
	AttackDamage = 20.f;
}

// Called when the game starts or when spawned
void ABossMk::BeginPlay()
{
	Super::BeginPlay();

	FTransform SocketTransform = GetMesh()->GetSocketTransform(TEXT("hand_rSocket"), ERelativeTransformSpace::RTS_World);
	// Sword를 스폰함
	// 스폰된 보스칼을 담아두고 싶다
	BossSword = GetWorld()->SpawnActor<ASword>(SwordFactory, SocketTransform);
	UPrimitiveComponent* SwordRoot = Cast<UPrimitiveComponent>(BossSword->GetRootComponent());
	if (SwordRoot != nullptr)
	{
		SwordRoot->IgnoreActorWhenMoving(this,true);
	}
	// SwordRoot->SetCollisionProfileName(TEXT("NoCollision"));  
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
}
// Called to bind functionality to input
void ABossMk::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABossMk::DamageAnimation()
{
	if (DamageMontage && GetMesh())
	{
		UE_LOG(LogTemp, Display, TEXT("DamageMontage && GetMesh"));
		
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(DamageMontage);
			UE_LOG(LogTemp, Display, TEXT("AnimInstance"));
		}
	}
}

void ABossMk::DestroyBoss()
{
	Destroy();
}

float ABossMk::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                          class AController* EventInstigator, AActor* DamageCauser)
{
	Hp -= DamageAmount;
	DamageAnimation();
	if (Hp < 0)
	{
		// 1초 뒤에 DestroyBoss 함수 호출
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ABossMk::DestroyBoss, 1.0f, false);
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);   
}

