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
	bool bHasEverSpawned = false;

	bool bIsActive = false;

	
public:	
	AEnemySpawner();

protected:
	virtual void BeginPlay() override;

	void CheckPlayerDistance();

	void SpawnEnemies();

	void ActivateEnemies();

	void DeactivateEnemies();


	FTimerHandle DistanceCheckHandle;

	bool bHasSpawned = false;

	UPROPERTY()
	TArray<AActor*> SpawnedEnemies;

public:
	UPROPERTY()
	TArray<USceneComponent*> SpawnPoints;

	// What enemy to spawn
	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<AActor> EnemyClass;

	// Distance at which an enemy will spawn
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (ClampMin = "0.0"))
	float SpawnRadius = 2000.f;

	// Despawn distance
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (ClampMin = "0.0"))
	float DespawnRadius = 3000.f;



};
