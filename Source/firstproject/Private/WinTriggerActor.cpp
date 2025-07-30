#include "WinTriggerActor.h"
#include "Components/BoxComponent.h"
#include "AMyTimedGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AWinTriggerActor::AWinTriggerActor()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;
    TriggerBox->SetBoxExtent(FVector(100.f));
    TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AWinTriggerActor::OnOverlapBegin);
}

void AWinTriggerActor::BeginPlay()
{
    Super::BeginPlay();
}

void AWinTriggerActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    // Only continue if overlapping actor is a character
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (!Character) return;

    // Access Blueprint variables via reflection
    UClass* CharClass = Character->GetClass();

    // Get collectedAmt
    FProperty* CollectedAmtProp = CharClass->FindPropertyByName(TEXT("collectedAmt"));
    FIntProperty* IntProp = CastField<FIntProperty>(CollectedAmtProp);
    int32 CollectedAmt = 0;

    if (IntProp)
    {
        CollectedAmt = IntProp->GetPropertyValue_InContainer(Character);
    }

    // If enough collected, set gameWon
    if (CollectedAmt >= 15)
    {
        FProperty* GameWonProp = CharClass->FindPropertyByName(TEXT("gameWon"));
        FBoolProperty* BoolProp = CastField<FBoolProperty>(GameWonProp);

        if (BoolProp)
        {
            BoolProp->SetPropertyValue_InContainer(Character, true);
            UE_LOG(LogTemp, Warning, TEXT("Game Won set to true!"));
            if (auto GM = Cast<AMyTimedGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
            {
                GM->bHasWon = true;
                UE_LOG(LogTemp, Warning, TEXT("GameMode.bHasWon set to true"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to find 'gameWon' boolean variable"));
        }
    }
}
