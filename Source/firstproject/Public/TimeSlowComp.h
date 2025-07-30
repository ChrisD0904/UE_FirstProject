#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimeSlowComp.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FIRSTPROJECT_API UTimeSlowComp : public UActorComponent
{
    GENERATED_BODY()

public:
    UTimeSlowComp();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

private:
    // Time‑slow state
    bool bIsActive = false;

    // Current and max charge
    UPROPERTY(EditDefaultsOnly, Category = "TimeSlow")
    float MaxCharge = 5.0f;

    UPROPERTY(VisibleAnywhere, Category = "TimeSlow")
    float Charge = 0.0f;

    // Rates (units/sec)
    UPROPERTY(EditDefaultsOnly, Category = "TimeSlow")
    float DrainRate = 1.0f;

    UPROPERTY(EditDefaultsOnly, Category = "TimeSlow")
    float RechargeRate = 0.5f;

    // Slow factor
    UPROPERTY(EditDefaultsOnly, Category = "TimeSlow", meta = (ClampMin = "0.1", ClampMax = "1.0"))
    float EnemyTimeDilation = 0.5f;

    // Cached enemies
    TArray<AActor*> AffectedEnemies;

    void ApplyTimeSlow();
    void RestoreTime();

public:
    // Blueprint‐callable API
    UFUNCTION(BlueprintCallable, Category = "TimeSlow")
    void ActivateTimeSlow();

    UFUNCTION(BlueprintCallable, Category = "TimeSlow")
    void DeactivateTimeSlow();

    UFUNCTION(BlueprintCallable, Category = "TimeSlow")
    float GetChargeRatio() const
    {
        return (MaxCharge > 0.f) ? (Charge / MaxCharge) : 0.f;
    }
};
