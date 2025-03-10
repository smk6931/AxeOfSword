﻿#include "AOSPlayerState.h"

#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "AxeOfSword/SM/GAS/AOSAbilitySystemComponent.h"
#include "AxeOfSword/SM/GAS/Attribute/BaseAttribute.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"
#include "GameFramework/SpringArmComponent.h"

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
		AbilitySystemComponent->RegisterGameplayTagEvent(
			AOSGameplayTags::Status_Combat, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnCombat);
	}
}

void AAOSPlayerState::OnCombat(const FGameplayTag CallbackTag, int32 NewCount)
{
	// Tag가 추가되는 경우에 대해 Camera Combat Mode가 활성화됨
	if (NewCount > 0 && !IsCombatMode)
	{
		IsCombatMode = true;
		APlayerCharacter* Player = Cast<APlayerCharacter>(GetPawn());
		if (!Player)
		{
			return;
		}
		Player->GetSpringArm()->CameraLagSpeed = CombatCameraLag;
	} else {
		FTimerManager* TimerManager = &GetWorld()->GetTimerManager();
		TimerManager->ClearTimer(CombatEndTimerHandle);
		TimerManager->SetTimer(CombatEndTimerHandle,
			FTimerDelegate::CreateUObject(this, &ThisClass::OnCombatEnd),
			CombatEndCooldown, false);
	}
}

void AAOSPlayerState::OnCombatEnd()
{
	IsCombatMode = false;
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetPawn());
	if (!Player)
	{
		return;
	}
	Player->GetSpringArm()->CameraLagSpeed = DefaultCameraLag;
}

UAbilitySystemComponent* AAOSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
