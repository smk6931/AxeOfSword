#include "PlayerCameraComponent.h"

UPlayerCameraComponent::UPlayerCameraComponent()
{
	SetRelativeLocation({60, 0, 0});
	bUsePawnControlRotation = true;
}

void UPlayerCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	DefaultLocation = GetRelativeLocation();
	DefaultFov = FieldOfView;
}

void UPlayerCameraComponent::MoveCameraOption(const FVector& MoveTo, const uint8 Fov)
{
	AddLocalOffset(MoveTo);
	FieldOfView = Fov;
}

void UPlayerCameraComponent::RollbackToFirstCameraOption()
{
	SetRelativeLocation(DefaultLocation);
	FieldOfView = DefaultFov;
}
