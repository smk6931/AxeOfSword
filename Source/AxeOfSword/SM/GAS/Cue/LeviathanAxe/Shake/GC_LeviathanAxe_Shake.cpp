#include "GC_LeviathanAxe_Shake.h"

#include "AbilitySystemComponent.h"
#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Character/Component/EquipComponent.h"
#include "AxeOfSword/SM/Helper/GameplayTagHelper.h"
#include "AxeOfSword/SM/Weapon/LeviathanAxe.h"

AGC_LeviathanAxe_Shake::AGC_LeviathanAxe_Shake()
{
	ShakeTimeline = CreateDefaultSubobject<UTimelineComponent>("Shake Timeline");
}

void AGC_LeviathanAxe_Shake::BeginPlay()
{
	Super::BeginPlay();
	
	ShakeCallback.BindDynamic(this, &ThisClass::OnShakeCallback);
	ShakeFinish.BindDynamic(this, &ThisClass::OnShakeFinish);
	
	ShakeTimeline->SetLooping(false);
	ShakeTimeline->AddInterpVector(ShakeTimingCurve, ShakeCallback);
	ShakeTimeline->SetTimelineFinishedFunc(ShakeFinish);
}

bool AGC_LeviathanAxe_Shake::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (Super::OnExecute_Implementation(MyTarget, Parameters))
	{
		return true;
	}
	
	BaseOwner = Cast<ABaseCharacter>(MyTarget);

	if (!BaseOwner)
	{
		return true;
	}
	
	ALeviathanAxe* TargetWeapon = Cast<ALeviathanAxe>(BaseOwner->GetEquipComponent()->GetMainWeapon());

	if (!TargetWeapon)
	{
		return true;
	}

	BaseRotation = TargetWeapon->GetWeaponMesh()->GetRelativeRotation();
	
	ShakeTimeline->PlayFromStart();
	
	return false;
}

void AGC_LeviathanAxe_Shake::OnShakeCallback(FVector Output)
{
	ALeviathanAxe* TargetWeapon = Cast<ALeviathanAxe>(BaseOwner->GetEquipComponent()->GetMainWeapon());

	if (!TargetWeapon)
	{
		return;
	}

	TargetWeapon->GetWeaponMesh()->SetRelativeRotation(
	{
		BaseRotation.Pitch + Output.X * 20,
		BaseRotation.Yaw + Output.Z * 20,
		BaseRotation.Roll + Output.Y * 20
	});
}

void AGC_LeviathanAxe_Shake::OnShakeFinish()
{
	BaseOwner->GetAbilitySystemComponent()->ExecuteGameplayCue(AOSGameplayTags::GameplayCue_Leviathan_TurnBack);
}
