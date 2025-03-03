#include "GC_SpawnNiagaraEffect.h"
#include "NiagaraFunctionLibrary.h"

bool UGC_SpawnNiagaraEffect::OnExecute_Implementation(AActor* MyTarget
	, const FGameplayCueParameters& Parameters) const
{
	UNiagaraFunctionLibrary::SpawnSystemAttached(Effect, MyTarget->GetRootComponent(), FName(""),
		FVector(0.f), FRotator(0.f), EAttachLocation::Type::SnapToTarget, true);
	
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
