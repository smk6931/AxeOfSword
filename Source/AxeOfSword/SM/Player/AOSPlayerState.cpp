#include "AOSPlayerState.h"

#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "AxeOfSword/SM/Character/Component/PlayerCameraComponent.h"
#include "AxeOfSword/SM/GAS/AOSAbilitySystemComponent.h"
#include "AxeOfSword/SM/GAS/Attribute/BaseAttribute.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"

AAOSPlayerState::AAOSPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAOSAbilitySystemComponent>("Ability System Component");
	Attribute = CreateDefaultSubobject<UBaseAttribute>("Attribute");
}

void AAOSPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(AbilitySystemComponent))
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->
			GetGameplayAttributeValueChangeDelegate(Attribute->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
		AbilitySystemComponent->RegisterGameplayTagEvent(
			AOSGameplayTags::Status_Combat, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnCombat);
	}
}

void AAOSPlayerState::OnCombat(const FGameplayTag CallbackTag, int32 NewCount)
{
	UE_LOG(LogTemp, Display, TEXT("%d"), NewCount);
	
	FTimerManager* TimerManager = &GetWorld()->GetTimerManager();
	TimerManager->ClearTimer(CombatEndTimerHandle);
	TimerManager->SetTimer(CombatEndTimerHandle,
		FTimerDelegate::CreateUObject(this, &ThisClass::OnCombatEnd),
		CombatEndCooldown, false);

	// Tag가 추가되는 경우에 대해 Camera Combat Mode가 활성화됨
	if (NewCount < 0 && !IsCombatMode)
	{
		IsCombatMode = true;
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
		if (!IsValid(PlayerCharacter))
		{
			return;
		}
		PlayerCharacter->GetCameraComponent()->EnableCombatCameraMode();
	}
}

void AAOSPlayerState::OnCombatEnd()
{
	IsCombatMode = false;
	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	PlayerCharacter->GetCameraComponent()->RollbackToFirstCameraOption();
}

void AAOSPlayerState::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	// TODO : 체력 0이 되면 처리할 내용 작성
}

UAbilitySystemComponent* AAOSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UBaseAttribute* AAOSPlayerState::GetAttributeSetBase() const
{
	return Attribute;
}

float AAOSPlayerState::GetHealth() const
{
	return Attribute->GetHealth();
}
