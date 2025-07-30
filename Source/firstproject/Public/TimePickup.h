// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "TimePickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class FIRSTPROJECT_API ATimePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Pickup")
	class USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Pickup|FX")
	UParticleSystem* CollectFX;

	UPROPERTY(EditAnywhere, Category = "Pickup|FX")
	USoundBase* CollectSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float TimeAmount = 30.0;


};
