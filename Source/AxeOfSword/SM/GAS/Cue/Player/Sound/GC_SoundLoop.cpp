#include "GC_SoundLoop.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

AGC_SoundLoop::AGC_SoundLoop()
{
}

void AGC_SoundLoop::BeginPlay()
{
	Super::BeginPlay();
	AudioComponent = UGameplayStatics::SpawnSoundAttached(SoundToPlay, GetRootComponent());
}

bool AGC_SoundLoop::OnExecute_Implementation(AActor* MyTarget
	, const FGameplayCueParameters& Parameters)
{
	if (AudioComponent)
	{
		AttachToActor(MyTarget, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		AudioComponent->Play();
	}
	
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}

bool AGC_SoundLoop::OnRemove_Implementation(AActor* MyTarget
	, const FGameplayCueParameters& Parameters)
{
	AudioComponent->Stop();
	
	return Super::OnRemove_Implementation(MyTarget, Parameters);
}
