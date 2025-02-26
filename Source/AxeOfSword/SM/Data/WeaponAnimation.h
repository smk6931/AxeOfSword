#pragma once

#include "CoreMinimal.h"
#include "AxeOfSword/SM/Helper/UtilHelper.h"
#include "Engine/DataAsset.h"
#include "WeaponAnimation.generated.h"

class ULevelSequence;
enum class EMoveDirection: uint8;

UCLASS()
class AXEOFSWORD_API UWeaponAnimation : public UDataAsset
{
	GENERATED_BODY()

public:
	GETTER(TArray<TObjectPtr<UAnimMontage>>, ComboAttackAnim)
	GETTER(TObjectPtr<UAnimMontage>, HeavyAttackAnim)
	GETTER(TObjectPtr<UAnimMontage>, ThrowAttackAnim)
	GETTER(TObjectPtr<ULevelSequence>, ExecutionCinematic)
	
	FORCEINLINE TMap<EMoveDirection, TObjectPtr<UAnimMontage>> GetDamagedAnim() { return DamagedAnim; }
	FORCEINLINE TMap<EMoveDirection, TObjectPtr<UAnimMontage>> GetEvadeAnim() { return EvadeAnim; }
	FORCEINLINE TMap<EMoveDirection, TObjectPtr<UAnimMontage>> GetRollAnim() { return RollAnim; }
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Option|Weapon", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> ComboAttackAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option|Weapon", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> HeavyAttackAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option|Weapon", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> ThrowAttackAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option|Weapon", meta = (AllowPrivateAccess = true))
	TMap<EMoveDirection, TObjectPtr<UAnimMontage>> DamagedAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option|Weapon", meta = (AllowPrivateAccess = true))
	TMap<EMoveDirection, TObjectPtr<UAnimMontage>> EvadeAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option|Weapon", meta = (AllowPrivateAccess = true))
	TMap<EMoveDirection, TObjectPtr<UAnimMontage>> RollAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Option|Weapon", meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequence> ExecutionCinematic;
};