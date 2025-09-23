#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class FIRSTPROJECT_API AEnemyBase : public ACharacter
{
    GENERATED_BODY()

public:
    AEnemyBase();

    // List of powerup classes that drop 
    UPROPERTY(EditDefaultsOnly, Category = "Drops")
    TArray<TSubclassOf<AActor>> PowerupsToDrop;

    // Odds of anything dropping
    UPROPERTY(EditDefaultsOnly, Category = "Drops", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float DropChance = 0.5f;

protected:
    virtual void Destroyed() override;
};
