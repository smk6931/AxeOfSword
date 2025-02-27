#pragma once

class FEnumHelper
{
public:
	template <typename EnumType>
	static FString GetClassEnumKeyAsString(const EnumType EnumeratorValue)
	{
		TArray<FString> EnumKey;
		UEnum::GetValueAsString(EnumeratorValue).
			ParseIntoArray(EnumKey, TEXT("::"));

		if (EnumKey.Num() == 1)
		{
			return EnumKey[0];
		}
		return EnumKey[1];
	}
};

UENUM()
enum class EAbilityInputId : uint8
{
	Undefined,
	Attack,
	CloseHold,
	TurnBack,
	Sprint,
	Damaged,
	Evade,
	Execution
};

UENUM()
enum class ECharacterState : uint8
{
	Idle,
	Attack,
	AttackIng,
	Casting,
	Damaged,
	WeaponTurnBack,
	Died,
};

UENUM()
enum class ELeviathanAxeState : uint8
{
	Idle,
	Throw,
	Return,
	Thrown_Idle,
};

UENUM()
enum class EMoveDirection : uint8
{
	Forward,
	ForwardRight,
	ForwardLeft,
	Left,
	Right,
	Backward,
	BackwardRight,
	BackwardLeft
};