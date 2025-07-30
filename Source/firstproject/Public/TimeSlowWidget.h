#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"    
#include "TimeSlowWidget.generated.h"

UCLASS()
class FIRSTPROJECT_API UTimeSlowWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // Binds to the ProgressBar in UMG
    UPROPERTY(meta = (BindWidget))
    UProgressBar* TimeSlowBar;

    // Called to update the bar
    UFUNCTION(BlueprintCallable, Category = "TimeSlow")
    void UpdateTimeSlowBar(float Ratio);

protected:
    virtual void NativeConstruct() override;
};
