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
};

UENUM()
enum class ELeviathanAxeStatus : uint8
{
	Idle,
	Throw,
	Return,
	Thrown_Idle,
};
