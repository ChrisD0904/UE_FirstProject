// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBaseplayerController.h"
#include "Kismet/GameplayStatics.h"
#include "AMyTimedGameMode.h"
#include "Blueprint/UserWidget.h"

void AMyBaseplayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	FInputActionBinding& PauseBinding =
		InputComponent->BindAction(
			"PauseGame",
			IE_Pressed,
			this,
			&AMyBaseplayerController::TogglePauseGame
		);

	// Allow this binding to fire even when the game is paused
	PauseBinding.bExecuteWhenPaused = true;
}


void AMyBaseplayerController::TogglePauseGame() {
    UWorld* World = GetWorld();
    if (!World) {
        return;
    }

    bool bCurrentlyPaused = UGameplayStatics::IsGamePaused(World);

    // Cast to your custom GameMode
    AMyTimedGameMode* GM = Cast<AMyTimedGameMode>(UGameplayStatics::GetGameMode(World));
    if (GM && GM->IsGameOver())
    {
        // If game is over and paused, prevent unpausing
        if (bCurrentlyPaused)
        {
            return; // Block unpause
        }
    }

    // Normal pause toggle logic below...
    UGameplayStatics::SetGamePaused(World, !bCurrentlyPaused);

    if (!bCurrentlyPaused) { // we just paused

        if (PauseMenuClass) {
            PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
            if (PauseMenuInstance) {
                PauseMenuInstance->AddToViewport();

                FInputModeGameAndUI InputMode;
                InputMode.SetWidgetToFocus(PauseMenuInstance->TakeWidget());
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                SetInputMode(InputMode);

                SetShowMouseCursor(true);
            }
        }

    }
    else { // we just unpaused
        if (PauseMenuInstance) {
            PauseMenuInstance->RemoveFromParent();
            PauseMenuInstance = nullptr;
        }

        SetInputMode(FInputModeGameOnly());
        SetShowMouseCursor(false);
    }
}



