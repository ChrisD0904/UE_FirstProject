#include "TimeSlowComp.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

UTimeSlowComp::UTimeSlowComp()
{
    // Enable ticking
    PrimaryComponentTick.bCanEverTick = true;
}

void UTimeSlowComp::BeginPlay()
{
    Super::BeginPlay();
    Charge = MaxCharge; // start full
}

void UTimeSlowComp::ActivateTimeSlow()
{
    if (!bIsActive && Charge > 0.f)
    {
        bIsActive = true;
        ApplyTimeSlow();
    }
}

void UTimeSlowComp::DeactivateTimeSlow()
{
    if (bIsActive)
    {
        bIsActive = false;
        RestoreTime();
    }
}

void UTimeSlowComp::ApplyTimeSlow()
{
    AffectedEnemies.Empty();
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Enemy"), AffectedEnemies);
    for (AActor* Enemy : AffectedEnemies)
    {
        if (Enemy)
        {
            Enemy->CustomTimeDilation = EnemyTimeDilation;
        }
    }
}

void UTimeSlowComp::RestoreTime()
{
    for (AActor* Enemy : AffectedEnemies)
    {
        if (Enemy)
        {
            Enemy->CustomTimeDilation = 1.f;
        }
    }
    AffectedEnemies.Empty();
}

void UTimeSlowComp::TickComponent(float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsActive)
    {
        Charge -= DrainRate * DeltaTime;
        if (Charge <= 0.f)
        {
            Charge = 0.f;
            DeactivateTimeSlow();
        }
    }
    else
    {
        if (Charge < MaxCharge)
        {
            Charge = FMath::Min(MaxCharge, Charge + RechargeRate * DeltaTime);
        }
    }
}
