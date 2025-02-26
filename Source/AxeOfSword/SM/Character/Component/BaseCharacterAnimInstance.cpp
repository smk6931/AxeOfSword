#include "BaseCharacterAnimInstance.h"

#include "EquipComponent.h"
#include "AxeOfSword/SM/Character/BaseCharacter.h"
#include "AxeOfSword/SM/Helper/StateHelper.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBaseCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BaseCharacter = Cast<ABaseCharacter>(GetOwningActor());
}

void UBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!BaseCharacter)
	{
		return;
	}

	SetMovementInfo();
	SetCombatInfo();
}

void UBaseCharacterAnimInstance::SetMovementInfo()
{
	IsFalling = BaseCharacter->GetMovementComponent()->IsFalling();
	Velocity = BaseCharacter->GetMovementComponent()->Velocity;
	GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);

	const FVector ForwardVector = BaseCharacter->GetActorForwardVector();
	const FVector VelocityDir = Velocity.GetSafeNormal();

	const float DotProduct = FVector::DotProduct(ForwardVector, VelocityDir);
	// 외적의 Z는 x,y축의 행렬 합이 되기 때문에 좌 우를 알 수 있는 요소가 된다.
	const float CrossZ =  FVector::CrossProduct(ForwardVector, VelocityDir).Z;

	// 외적의 Z가 0보다 크거나 같다면 오른쪽, 아니라면 왼쪽이다라고 생각하면 된다.
	VelocityArc = UKismetMathLibrary::DegAcos(DotProduct) * (CrossZ >= 0 ? 1 : -1);
}

void UBaseCharacterAnimInstance::SetCombatInfo()
{
	IsZoomIn = UStateHelper::IsZoomIn(BaseCharacter->GetAbilitySystemComponent());
	IsWaitForWeaponTurnBack = UStateHelper::IsWaitForTurnBackWeapon(BaseCharacter);
	if (BaseCharacter->GetEquipComponent())
	{
		// 손에든 무기의 주인이 아닌 경우가 손이 현재 비어있는 경우임을 의미한다.
		IsHandFree = !BaseCharacter->GetEquipComponent()->IsMainWeaponOwner();
	}
}
