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

void UPlayerCameraComponent::SetCameraOption(const FVector& MoveTo, const int8 Fov)
{
	SetRelativeLocation(MoveTo);
	FieldOfView = Fov;
}

void UPlayerCameraComponent::RollbackToFirstCameraOption()
{
	SetRelativeLocation(DefaultLocation);
	FieldOfView = DefaultFov;
}
