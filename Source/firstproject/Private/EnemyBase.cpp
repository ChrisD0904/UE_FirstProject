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
    if (World && World->HasBegunPlay())  //Only runs after play starts
    {
        // Roll to see if anything will even drop
        if (PowerupsToDrop.Num() > 0 && FMath::FRand() <= DropChance)
        {
            // Pick one index at random
            int32 Index = FMath::RandRange(0, PowerupsToDrop.Num() - 1);
            TSubclassOf<AActor> ChosenClass = PowerupsToDrop[Index];

            // Spawn the collectible
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

    // Finally destroy the enemy 
    Super::Destroyed();
}
