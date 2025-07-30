#include "EnemyBase.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AEnemyBase::Destroyed()
{
    UWorld* World = GetWorld();
    if (World && World->HasBegunPlay())  // ← only after Play starts
    {
        // 1) Roll to see if we drop anything at all
        if (PowerupsToDrop.Num() > 0 && FMath::FRand() <= DropChance)
        {
            // 2) Pick one index at random
            int32 Index = FMath::RandRange(0, PowerupsToDrop.Num() - 1);
            TSubclassOf<AActor> ChosenClass = PowerupsToDrop[Index];

            // 3) Spawn it
            if (ChosenClass)
            {
                FActorSpawnParameters Params;
                Params.SpawnCollisionHandlingOverride =
                    ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

                GetWorld()->SpawnActor<AActor>(
                    ChosenClass,
                    GetActorLocation(),
                    FRotator::ZeroRotator,
                    Params
                );
            }
        }
    }

    // 4) Finally destroy the enemy as normal
    Super::Destroyed();
}
