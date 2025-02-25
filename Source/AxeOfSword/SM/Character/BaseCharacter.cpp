#include "BaseCharacter.h"

#include "AxeOfSword/SM/GAS/Attribute/BaseAttribute.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"
#include "AxeOfSword/SM/Helper/StateHelper.h"
#include "AxeOfSword/SM/Player/AOSPlayerState.h"
#include "AxeOfSword/SM/GAS/AOSAbilitySystemComponent.h"
#include "AxeOfSword/SM/Helper/EnumHelper.h"
#include "Component/EquipComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseCharacter::ABaseCharacter()
{
	CurrentState = ECharacterState::Idle;
	EquipComponent = CreateDefaultSubobject<UEquipComponent>("Equip Component");

	FistRightSphereCapsule = CreateDefaultSubobject<USphereComponent>("Fist Right Sphere Capsule");
	FistRightSphereCapsule->SetupAttachment(GetMesh());

	FistRightSphereCapsule->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_r");
	FistRightSphereCapsule->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = Attribute->GetMovementSpeed();
	
	HealthChangedDelegateHandle = AbilitySystemComponent->
		GetGameplayAttributeValueChangeDelegate(Attribute->GetHealthAttribute())
	.AddUObject(this, &ThisClass::OnHealthChanged);
	MovementSpeedChangedDelegateHandle = AbilitySystemComponent->
		GetGameplayAttributeValueChangeDelegate(Attribute->GetMovementSpeedAttribute())
	.AddUObject(this, &ThisClass::OnMovementSpeedChanged);
}

float ABaseCharacter::TakeDamage(float DamageAmount
	, FDamageEvent const& DamageEvent, AController* EventInstigator
	, AActor* DamageCauser)
{
	if (!AbilitySystemComponent)
	{
		return 0;
	}
	
	if (!Attribute)
	{
		return 0;
	}

	if (AOSGameplayTags::HasGameplayTag(AbilitySystemComponent, AOSGameplayTags::Status_Invincible))
	{
		return 0;
	}
	
	Attribute->SetHealth(Attribute->GetHealth() - DamageAmount);
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator
							, DamageCauser);
}

void ABaseCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue == 0)
	{
		AbilitySystemComponent->CancelAllAbilities();
		return;
	}

	// 이미 데미지를 받은 상태인 경우 굳이 뭘 하지 않는다.
	if (UStateHelper::IsDamaged(this))
	{
		return;
	}
	
	// NewValue가 작다는 것은 체력이 감소했음을 의미한다.
	if (Data.OldValue > Data.NewValue)
	{
		FGameplayTagContainer ActivateTag;
		ActivateTag.AddTag(AOSGameplayTags::Passive_Damaged);
		AbilitySystemComponent->TryActivateAbilitiesByTag(ActivateTag);
	}
}

void ABaseCharacter::OnMovementSpeedChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

float ABaseCharacter::GetHealth() const
{
	return Attribute->GetHealth();
}

void ABaseCharacter::ToggleFistAttackMode(const bool Toggle)
{
	FistRightSphereCapsule->SetCollisionEnabled(Toggle ?
		ECollisionEnabled::Type::QueryAndPhysics : ECollisionEnabled::Type::NoCollision);
}
