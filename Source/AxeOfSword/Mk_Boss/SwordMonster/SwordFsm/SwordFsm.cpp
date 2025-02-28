// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordFsm.h"


// Sets default values for this component's properties
USwordFsm::USwordFsm()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USwordFsm::BeginPlay()
{
	Super::BeginPlay();
	// GetWorld()->GetTimerManager().SetTimer(SwordTimer);
}


// Called every frame
void USwordFsm::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (SwordState)
	{
	case ESwordState::idle:
		Idle();
		break;
	case ESwordState::Move:
		Move();
		break;
	case ESwordState::OneAttack:
		OneAttactk();
		break;
	case ESwordState::TwiceAttack:
		TwiceAttackk();
		break;
	case ESwordState::ThirdAttack:
		ThirdAttackk();
		break;
	case ESwordState::Dash:
		Dash();
		break;
	}
}

void USwordFsm::Idle()
{
}

void USwordFsm::Move()
{
}

void USwordFsm::OneAttactk()
{
}

void USwordFsm::TwiceAttackk()
{
}

void USwordFsm::ThirdAttackk()
{
}

void USwordFsm::Dash()
{
}


void USwordFsm::SwitchSwordState()
{
}

