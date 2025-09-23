#include "AMyTimedGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Actor.h"
#include "Engine/PostProcessVolume.h"


AMyTimedGameMode::AMyTimedGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    bRedHueActive = false;
    DangerVolume = nullptr;
}

// When the game starts
void AMyTimedGameMode::BeginPlay()
{
    Super::BeginPlay();

    bIsGameOver = false;

    GetWorldTimerManager().SetTimer(
        GameTimerHandle,
        this,
        &AMyTimedGameMode::OnGameTimerExpired,
        75.0f,
        false
    );

    for (TActorIterator<APostProcessVolume> It(GetWorld()); It; ++It) {
        if (It->ActorHasTag("DangerPPV")) {
            DangerVolume = *It;
            DangerVolume->Settings.bOverride_ColorSaturation = true;
            //DangerVolume->Settings.ColorSaturation = FVector4(7.f, 0.05f, 0.05f, 1.f);
            DangerVolume->BlendWeight = 0.f;
            break;
        }
    }

    SpawnCoresRandomly();
}

// Randomly spawns cores by picking positions on the map I selected already
void AMyTimedGameMode::SpawnCoresRandomly()
{
    if (!ReactorClass)
    {
        UE_LOG(LogTemp, Error, TEXT("ReactorClass is not set!"));
        return;
    }

    TArray<AActor*> SpawnPoints;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), SpawnTag, SpawnPoints);

    if (SpawnPoints.Num() < NumReactors)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Not enough spawn points (%d) for %d reactors"),
            SpawnPoints.Num(), NumReactors);
    }

    for (int32 i = SpawnPoints.Num() - 1; i > 0; --i)
    {
        int32 j = FMath::RandRange(0, i);
        SpawnPoints.Swap(i, j);
    }

    for (int32 i = 0; i < NumReactors && i < SpawnPoints.Num(); ++i)
    {
        FTransform SpawnTM = SpawnPoints[i]->GetActorTransform();
        AActor* NewCore = GetWorld()->SpawnActor<AActor>(
            ReactorClass,
            SpawnTM
        );
        if (NewCore)
        {
            NewCore->Tags.Add("NuclearCore");
        }
    }
}


// Handles the explosion when timer runs out
void AMyTimedGameMode::OnGameTimerExpired()
{

    bIsGameOver = true;

    AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerActor)
    {
        FVector Location = PlayerActor->GetActorLocation();
        UGameplayStatics::PlaySound2D(GetWorld(), TimerExpiredSound);
        UE_LOG(LogTemp, Error, TEXT("ExplosionEffect is NULL or PlayerActor not found."));
    }

    if (PlayerActor && ExplosionEffect)
    {
        FVector Location = PlayerActor->GetActorLocation();
        FRotator Rotation = FRotator::ZeroRotator;

        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            ExplosionEffect,
            Location,
            Rotation,
            FVector(50.0f) 
        );

    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ExplosionEffect is NULL or PlayerActor not found."));
    }

    // Delay showing the UI
    GetWorldTimerManager().SetTimer(
        DelayHandle,
        this,
        &AMyTimedGameMode::ShowGameOverUIWithPause,
        0.3f,
        false
    );
}

void AMyTimedGameMode::ShowGameOverUIWithPause()
{
    if (GameOverWidgetClass)
    {
        GameOverWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
        if (GameOverWidgetInstance)
        {
            GameOverWidgetInstance->AddToViewport();

            APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PC)
            {
                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(GameOverWidgetInstance->TakeWidget());
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                PC->SetInputMode(InputMode);
                PC->SetShowMouseCursor(true);
            }

            UGameplayStatics::SetGamePaused(GetWorld(), true);

            UE_LOG(LogTemp, Warning, TEXT("Game Over UI displayed and game paused"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameOverWidgetClass not set in the editor!"));
    }
}


float AMyTimedGameMode::GetRemainingTime() const
{
    if (GetWorld())
    {
        float Rem = GetWorldTimerManager().GetTimerRemaining(GameTimerHandle);
        return FMath::Max(0.0f, Rem);
    }
    return 0.f;
}

void AMyTimedGameMode::AddTime(float ExtraSeconds) {
    // If the game is already over or no timer, ignore
    if (bIsGameOver || !GetWorldTimerManager().IsTimerActive(GameTimerHandle))
    {
        UE_LOG(LogTemp, Warning, TEXT("AddTime: No active timer or game over."));
        return;
    }

    float TimeLeft = GetWorldTimerManager().GetTimerRemaining(GameTimerHandle);
    float NewTimeLeft = TimeLeft + ExtraSeconds;

    GetWorldTimerManager().ClearTimer(GameTimerHandle);
    GetWorldTimerManager().SetTimer(GameTimerHandle, this,
        &AMyTimedGameMode::OnGameTimerExpired,
        NewTimeLeft,
        false
    );
}

// Function to handle the red flash when game is close to finishing
void AMyTimedGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bHasWon || bIsGameOver || !DangerVolume)
    {
        return;
    }
    bool bNowDanger = (GetRemainingTime() <= DangerThreshold);

    if (bNowDanger && !bRedHueActive)
    {
        bRedHueActive = true;
        DangerCycleStartTime = GetWorld()->TimeSeconds;
        if (DangerAlarmSound)
        {
            UGameplayStatics::PlaySound2D(GetWorld(), DangerAlarmSound);
        }
    }
    else if (!bNowDanger && bRedHueActive)
    {
        bRedHueActive = false;
    }

    if (!bNowDanger)
    {
        DangerVolume->BlendWeight = FMath::FInterpTo(
            DangerVolume->BlendWeight, 0.f, DeltaSeconds, 3.0f
        );
        return;
    }

    float Elapsed = GetWorld()->TimeSeconds - DangerCycleStartTime;
    float CycleTime = FMath::Fmod(Elapsed, FlashCycleDuration);
    float HalfCycle = FlashCycleDuration * 0.5f;

    float TargetBlend;
    if (CycleTime < HalfCycle)
    {
        TargetBlend = CycleTime / HalfCycle;
    }
    else
    {
        TargetBlend = 1.0f - ((CycleTime - HalfCycle) / HalfCycle);
    }

    DangerVolume->BlendWeight = TargetBlend;
}


