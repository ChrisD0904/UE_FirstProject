// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
    GetWorld()->GetTimerManager().SetTimer(
        DistanceCheckHandle,
        this,
        &AEnemySpawner::CheckPlayerDistance,
        0.2f,
        true
    );

    TArray<USceneComponent*> ChildrenComps;
    GetComponents<USceneComponent>(ChildrenComps);

    for (USceneComponent* Comp : ChildrenComps)
    {
        if (Comp->ComponentHasTag("SpawnPoint"))
        {
            SpawnPoints.Add(Comp);
        }
    }

}

void AEnemySpawner::CheckPlayerDistance()
{
    APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
    if (!Player) return;

    const float Dist = FVector::Dist(Player->GetActorLocation(), GetActorLocation());

    // 1) First‑time ever spawn
    if (!bHasEverSpawned && Dist <= SpawnRadius)
    {
        SpawnEnemies();
        bHasEverSpawned = true;
        bIsActive = true;
        return;  // don’t immediately deactivate
    }

    if (bHasEverSpawned)
    {
        if (!bIsActive && Dist <= SpawnRadius)
        {
            ActivateEnemies();
            bIsActive = true;
        }
        else if (bIsActive && Dist >= DespawnRadius)
        {
            DeactivateEnemies();
            bIsActive = false;
        }
    }
}


void AEnemySpawner::SpawnEnemies()
{
    UWorld* W = GetWorld();
    if (!W || !EnemyClass) return;

    for (USceneComponent* Point : SpawnPoints)
    {
        if (!Point) continue;
        FTransform T = Point->GetComponentTransform();

        APawn* P = W->SpawnActor<APawn>(EnemyClass, T);
        if (P)
        {
            P->SpawnDefaultController();  // hook up AI
            SpawnedEnemies.Add(P);
        }
    }
}

void AEnemySpawner::ActivateEnemies()
{
    for (AActor* A : SpawnedEnemies)
    {
        if (!IsValid(A)) continue;
        A->SetActorHiddenInGame(false);
        A->SetActorEnableCollision(true);
        A->SetActorTickEnabled(true);
        // if needed, re‑start any AI logic here
    }
}

void AEnemySpawner::DeactivateEnemies()
{
    for (AActor* A : SpawnedEnemies)
    {
        if (!IsValid(A)) continue;
        A->SetActorHiddenInGame(true);
        A->SetActorEnableCollision(false);
        A->SetActorTickEnabled(false);
        // optionally stop AI: 
        if (APawn* P = Cast<APawn>(A))
            if (P->Controller)
                P->Controller->StopMovement();
    }
}


/*void AEnemySpawner::DespawnEnemies() {
    for (AActor* Enemy : SpawnedEnemies) {
        if (IsValid(Enemy)) {
            Enemy->Destroy();
        }
    }
    SpawnedEnemies.Empty();
}*/