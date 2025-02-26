#pragma once

class FUtilHelper
{
	
};

#define GETTER(type, varName) \
UFUNCTION(BlueprintCallable) \
FORCEINLINE type Get##varName() const { return varName; }

#define SETTER(type, varName) \
UFUNCTION(BlueprintCallable) \
FORCEINLINE void Set##varName(const type& val) { varName = val; }

#define GETTER_SETTER(type, varName) \
GETTER(type, varName) \
SETTER(type, varName)