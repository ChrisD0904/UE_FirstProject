#include "TestPawn.h"

ATestPawn::ATestPawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATestPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Binds functionality to input
void ATestPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

