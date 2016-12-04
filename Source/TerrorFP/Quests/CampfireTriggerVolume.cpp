// Copyright @Maxpro 2016

#include "TerrorFP.h"
#include "../TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "ObjectiveChange.h" // TODO: Probably can remove this
#include "../Game/SurvivalSaveGame.h"
#include "CampfireTriggerVolume.h"
#include "Kismet/KismetStringLibrary.h"

ACampfireTriggerVolume::ACampfireTriggerVolume()
{
    //Register the enter and exit overlaps to fire
    OnActorBeginOverlap.AddDynamic(this, &ACampfireTriggerVolume::OnTriggerEnter);
    OnActorEndOverlap.AddDynamic(this, &ACampfireTriggerVolume::OnTriggerExit);
}

/*4.12 version*/
void ACampfireTriggerVolume::OnTriggerEnter(AActor* OverlapedActor, AActor* OtherActor)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("Begin overlap has fired"));
        
        ATP_ThirdPersonCharacter* Char = Cast<ATP_ThirdPersonCharacter>(OtherActor);
        if (Char)
        {
            if (Char->GetNumberOfWood() == 3)
            {
                SetCampfireObjective(Char);
            }
            else
            {
                // TODO: Maybe add a message pop-up here saying "Not enough firewood or something"
            }
        }
    }
}

/*4.12 version*/
void ACampfireTriggerVolume::OnTriggerExit(AActor* OverlapedActor, AActor* OtherActor)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("End overlap has fired"));
    }
}

void ACampfireTriggerVolume::SetCampfireObjective(ATP_ThirdPersonCharacter* Char)
{
    
    Char->PlayerObjective = NextQuest;
    if (UGameplayStatics::DoesSaveGameExist(ATP_ThirdPersonCharacter::PlayerSaveSlot, 0))
    {
        // TODO: Filter out into function
        // If it does exist
        SaverSubClass = (UGameplayStatics::LoadGameFromSlot(ATP_ThirdPersonCharacter::PlayerSaveSlot, 0));
        
        USurvivalSaveGame* SurvivalSaveGame = Cast<USurvivalSaveGame>(SaverSubClass);
        
        SurvivalSaveGame->SetPlayerObjective(FText::FromString(NextQuest));
        
        bool save = UGameplayStatics::SaveGameToSlot(SaverSubClass, ATP_ThirdPersonCharacter::PlayerSaveSlot, 0);
        
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, "Save success ? " +
                                             UKismetStringLibrary::Conv_BoolToString(save));
        
    }
    else
    {
        // TODO: Filter out into function
        // If there is no save game.
        SaverSubClass = UGameplayStatics::CreateSaveGameObject(USurvivalSaveGame::StaticClass());
        
        USurvivalSaveGame* SurvivalSaveGame = Cast<USurvivalSaveGame>(SaverSubClass);
        
        SurvivalSaveGame->SetPlayerObjective(FText::FromString(NextQuest));
        
        bool save = UGameplayStatics::SaveGameToSlot(SaverSubClass, ATP_ThirdPersonCharacter::PlayerSaveSlot, 0);
        
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, "Save success ? " +
                                             UKismetStringLibrary::Conv_BoolToString(save));
    }
}