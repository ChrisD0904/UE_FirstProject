// BPI_Healable.h
#pragma once
#include "UObject/Interface.h"
#include "BPI_Healable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UBPI_Healable : public UInterface
{
    GENERATED_BODY()
};

class IBPI_Healable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
    void AddHealth(float Amount); 
};
