#include "TutorialCharacter.h"


ATutorialCharacter::ATutorialCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATutorialCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATutorialCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATutorialCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATutorialCharacter::SpawnActor() {

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(actorBPToSpawn, spawnPos, spawnParams);
}

