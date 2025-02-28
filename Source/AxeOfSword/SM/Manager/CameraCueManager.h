#pragma once

class APlayerCharacter;
class UAbilitySystemComponent;

class FCameraCueManager
{
public:
	static void CastAttackCameraShakeByPlayerAttack(UAbilitySystemComponent* ASC, const APlayerCharacter* Player);
};
