#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WinTriggerActor.generated.h"

class UBoxComponent;

UCLASS()
class FIRSTPROJECT_API AWinTriggerActor : public AActor
{
    GENERATED_BODY()

public:
    AWinTriggerActor();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    UBoxComponent* TriggerBox;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);
};
