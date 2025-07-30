// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "UObject/UnrealType.h"  // for FFloatProperty
#include "Kismet/GameplayStatics.h"
#include "HealthPickup.h"
#include "BPI_Healable.h"
#include "Components/SphereComponent.h"

// Sets default values
AHealthPickup::AHealthPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetSphereRadius(100.f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComponent->SetupAttachment(RootComponent);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AHealthPickup::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHealthPickup::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    if (!OtherActor || OtherActor == this) return;

    if (OtherActor->GetClass()->ImplementsInterface(UBPI_Healable::StaticClass()))
    {
        IBPI_Healable::Execute_AddHealth(OtherActor, (float)HealAmount);
        UE_LOG(LogTemp, Warning, TEXT("Healed %s by %d"), *OtherActor->GetName(), HealAmount);

        if (CollectFX)
        {
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                CollectFX,
                GetActorLocation(),
                FRotator::ZeroRotator
            );
        }

        // Play collection SFX
        if (CollectSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                GetWorld(),
                CollectSound,
                GetActorLocation()
            );
        }

        Destroy();
    }
}


// Called every frame
/*void AHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

