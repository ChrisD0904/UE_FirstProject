// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthPickup.generated.h"

UCLASS()
class FIRSTPROJECT_API AHealthPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

public:	
		UPROPERTY(EditAnywhere)
		class USphereComponent* CollisionComponent;

		UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* MeshComponent;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		int32 HealAmount = 30;

		UPROPERTY(EditAnywhere, Category = "Pickup|FX")
		UParticleSystem* CollectFX;

		UPROPERTY(EditAnywhere, Category = "Pickup|FX")
		USoundBase* CollectSound;




};
