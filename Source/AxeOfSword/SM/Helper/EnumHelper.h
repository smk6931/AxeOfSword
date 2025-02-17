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
