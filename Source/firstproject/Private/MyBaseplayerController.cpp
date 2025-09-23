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

	PauseBinding.bExecuteWhenPaused = true;
}


void AMyBaseplayerController::TogglePauseGame() {
    UWorld* World = GetWorld();
    if (!World) {
        return;
    }

    bool bCurrentlyPaused = UGameplayStatics::IsGamePaused(World);

    // Cast to custom GameMode
    AMyTimedGameMode* GM = Cast<AMyTimedGameMode>(UGameplayStatics::GetGameMode(World));
    if (GM && GM->IsGameOver())
    {
        if (bCurrentlyPaused)
        {
            return; // Block unpause
        }
    }

    UGameplayStatics::SetGamePaused(World, !bCurrentlyPaused);

    if (!bCurrentlyPaused) { // Paused

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
    else { // Unpaused
        if (PauseMenuInstance) {
            PauseMenuInstance->RemoveFromParent();
            PauseMenuInstance = nullptr;
        }

        SetInputMode(FInputModeGameOnly());
        SetShowMouseCursor(false);
    }
}



