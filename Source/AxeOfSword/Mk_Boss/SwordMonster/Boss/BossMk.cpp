#include "BossMk.h"
#include "TimerManager.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/BossAnim/BossAnim.h"
#include "AxeOfSword/Mk_Boss/SwordMonster/Sword/Sword.h"
#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

class UBossHpWidget;
// Sets default values
ABossMk::ABossMk()
{
	PrimaryActorTick.bCanEverTick = true;

	Fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("Fsm"));
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempBossMesh(
		TEXT("'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	if (TempBossMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(TempBossMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90),
			FRotator(0,-90,0));
	}
}

// Called when the game starts or when spawned
void ABossMk::BeginPlay()
{
	Super::BeginPlay();
	
	BossAnim = Cast<UBossAnim>(GetMesh()->GetAnimInstance());

	FTransform SocketTransform = GetMesh()->GetSocketTransform(TEXT("hand_rSocket"), ERelativeTransformSpace::RTS_World);
	// Sword를 스폰함
	// 스폰된 보스칼을 담아두고 싶다
	BossSword = GetWorld()->SpawnActor<ASword>(SwordFactory, SocketTransform);
	UPrimitiveComponent* SwordRoot = Cast<UPrimitiveComponent>(BossSword->GetRootComponent());
	if (SwordRoot != nullptr)
	{
		//스워드 루트의 액터가 움직일시 충돌을 소유액터와 무시하고싶다 =
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
}
// Called to bind functionality to input
void ABossMk::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
// 몬스터, 칼 삭제 
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
	if (Fsm->mState == EEnemyState::Move)
	{   // Ilde로 전환
		Fsm->mState = EEnemyState::idle;
		BossAnim->animState = EEnemyState::idle;
	}
	Hp -= DamageAmount;
	if (ExcutionGuage > 0)
	{   
		ExcutionGuage -= DamageAmount;
	}
	else
	{
		ExcutionGuage = 0;
	}

	UGameplayStatics::PlaySound2D(GetWorld(), HitSound, 0.8, 0.9, 0, nullptr, this);

	// 데미지 받을 시 뒤로 밀려나는 넉백 기능 강제 구현
	LaunchCharacter(DamageCauser->GetActorForwardVector() * 750,
		true, true);
	
	if (BossSword->SwordCapsule->GetCollisionEnabled() == ECollisionEnabled::Type::NoCollision)
	{   //스워드 캡슐의 콜리전이 비활성화 되어있을때 오버랩 이벤트 발생시 데미지 애니메이션 실행
		DamageAnimation();
		
		//Idle 상태로 전환
		BossAnim->animState = EEnemyState::idle;
	}
	if (Hp < 1)
	{
		//몬스터 사망
		Hp = 0;
		PlayDieAnimation();
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ABossMk::DestroyBossSword, 0.5f, true);
		// 1초 뒤에 DestroyBoss 함수 호출
		GetWorldTimerManager().SetTimer(TimerHandleB, this, &ABossMk::DestroyBoss, 2.2f, true);
	}
	// HpBar업데이트
	BlueTakeDamage();
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
