#include "TimePickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AMyTimedGameMode.h"

// Sets default values
ATimePickup::ATimePickup()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(25.f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComponent->SetGenerateOverlapEvents(true);
	RootComponent = CollisionComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATimePickup::OnOverlapBegin);
}

void ATimePickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATimePickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	if (!OtherActor || OtherActor == this)
		return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (OtherActor != PlayerPawn)
		return;

	AMyTimedGameMode* GM = Cast<AMyTimedGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->AddTime(TimeAmount);
		UE_LOG(LogTemp, Log, TEXT("TimePickup: Added %.1f seconds to timer."), TimeAmount);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TimePickup: Could not find AMyTimedGameMode."));
	}

	if (CollectFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			CollectFX,
			GetActorLocation(),
			FRotator::ZeroRotator
		);
	}

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

