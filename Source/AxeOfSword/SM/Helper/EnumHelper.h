#pragma once

class FEnumHelper
{
};

UENUM()
enum class EAbilityInputId : uint8
{
	Undefined,
	Attack,
	CloseHold,
	TurnBack,
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
