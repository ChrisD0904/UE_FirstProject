#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"
#include "Particles/ParticleSystem.h"
#include "Engine/PostProcessVolume.h"
#include "Sound/SoundBase.h"
#include "AMyTimedGameMode.generated.h"


UCLASS()
class FIRSTPROJECT_API AMyTimedGameMode : public AGameModeBase
{
    GENERATED_BODY()
private:
    bool bIsGameOver;

    bool bRedHueActive;

    APostProcessVolume* DangerVolume;

    static constexpr float DangerThreshold = 15.0f;

    void SpawnCoresRandomly();

    float DangerCycleStartTime = 0.f;
    
    static constexpr float FlashCycleDuration = 2.0f;
   
protected:
    virtual void BeginPlay() override;


    FTimerHandle DelayHandle;

    UFUNCTION()
    void OnGameTimerExpired();

    UFUNCTION()
    void ShowGameOverUIWithPause();


public:
   
    UPROPERTY(BlueprintReadWrite, Category = "Game Flow")
    bool bHasWon = false;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    USoundBase* DangerAlarmSound;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    USoundBase* TimerExpiredSound;


    AMyTimedGameMode();
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    UParticleSystem* ExplosionEffect;

    UFUNCTION(BlueprintCallable, Category = "Game Timer")
    float GetRemainingTime() const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FTimerHandle GameTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> GameOverWidgetClass;

    UUserWidget* GameOverWidgetInstance;

    bool IsGameOver() const { return bIsGameOver; }

    UFUNCTION(BlueprintCallable, Category = "Game Timer")
    void AddTime(float ExtraSeconds);

    UPROPERTY(EditAnywhere, Category = "Reactors")
    TSubclassOf<AActor> ReactorClass;

    UPROPERTY(EditAnywhere, Category = "Reactors", meta = (ClampMin = 1))
    int32 NumReactors = 15;

    UPROPERTY(EditAnywhere, Category = "Reactors")
    FName SpawnTag = "CoreSpawn";

};
