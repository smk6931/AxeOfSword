#include "GC_FovMove.h"

#include "AxeOfSword/SM/Character/PlayerCharacter.h"
#include "AxeOfSword/SM/Character/Component/PlayerCameraComponent.h"


AGC_FovMove::AGC_FovMove()
{
	SetHidden(true);
	FovMoveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Fov Move Timeline Component"));
}

void AGC_FovMove::BeginPlay()
{
	Super::BeginPlay();
	
	FovMoveCallback.BindDynamic(this, &ThisClass::OnFovMoveCallback);
	FovMoveFinish.BindDynamic(this, &ThisClass::OnFovMoveFinish);
	
	FovMoveTimeline->SetLooping(false);
	FovMoveTimeline->AddInterpFloat(FovMoveTimingCurve, FovMoveCallback);
	FovMoveTimeline->SetTimelineFinishedFunc(FovMoveFinish);
}

bool AGC_FovMove::OnExecute_Implementation(AActor* MyTarget
	, const FGameplayCueParameters& Parameters)
{
	if (Super::OnExecute_Implementation(MyTarget, Parameters))
	{
		return true;
	}

	Target = MyTarget;
	
	IsReverse = Parameters.AbilityLevel != 1;

	if (IsReverse)
	{
		FovMoveTimeline->ReverseFromEnd();
	} else
	{
		FovMoveTimeline->PlayFromStart();
	}
	
	return false;
}

void AGC_FovMove::OnFovMoveCallback(float Output)
{
	const APlayerCharacter* Player = Cast<APlayerCharacter>(Target);
	if (!Player)
	{
		return;
	}
	
	UPlayerCameraComponent* PlayerCameraComponent = Player->GetCameraComponent();

	PlayerCameraComponent->SetFov(FMath::Lerp(PlayerCameraComponent->GetDefaultFov(),
		FovTo, Output));
}

void AGC_FovMove::OnFovMoveFinish()
{
	if (IsRollback)
	{
		FovMoveTimeline->Reverse();
	}
}