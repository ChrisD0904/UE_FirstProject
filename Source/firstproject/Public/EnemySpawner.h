// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class FIRSTPROJECT_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

private:
	/** Have we ever spawned (for the first‑time only)? */
	bool bHasEverSpawned = false;

	/** Are enemies currently active (shown & ticking)? */
	bool bIsActive = false;

	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CheckPlayerDistance();

	/** Only called once ever to populate the pool */
	void SpawnEnemies();

	/** Show, tick, and collide your pooled enemies */
	void ActivateEnemies();

	/** Hide, untick, and disable collision */
	void DeactivateEnemies();


	FTimerHandle DistanceCheckHandle;

	bool bHasSpawned = false;

	UPROPERTY()
	TArray<AActor*> SpawnedEnemies;

public:
	UPROPERTY()
	TArray<USceneComponent*> SpawnPoints;

	/** What enemy to spawn */
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<AActor> EnemyClass;

	/** Distance at which to spawn */
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (ClampMin = "0.0"))
	float SpawnRadius = 2000.f;

	/** Distance at which to despawn (should be ≥ SpawnRadius) */
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (ClampMin = "0.0"))
	float DespawnRadius = 3000.f;



};
