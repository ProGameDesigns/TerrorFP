// Copyright @Maxpro 2016

#include "TerrorFP.h"
#include "../TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "ObjectiveComplete.h"




UObjectiveComplete::UObjectiveComplete(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{
    static ConstructorHelpers::FObjectFinder<UTexture2D> ObjBanner(TEXT("/Game/SurvivalHorrorGame/ObjectiveComplete"));
    ObjCompleteBanner = ObjBanner.Object;
//
//    FirstBanner = GetWidgetFromName("FirstQuestImg");
//    WidgetAnim = Cast<UWidgetAnimation>(FirstBanner);
    
}

void UObjectiveComplete::NativeConstruct()
{
    Super::NativeConstruct();
    AssignAnimations();
    PlayAnimation(WidgetAnim);
    
}

void UObjectiveComplete::AssignAnimations()
{
    UProperty* prop = GetClass()->PropertyLink;
    
    // Run through all properties of this class to find any widget animations
    while( prop != nullptr  )
    {
        // Only interested in object properties
        if( prop->GetClass() == UObjectProperty::StaticClass() )
        {
            UObjectProperty* objectProp = Cast<UObjectProperty>(prop);
            
            // Only want the properties that are widget animations
            if( objectProp->PropertyClass == UWidgetAnimation::StaticClass() )
            {
                UObject* object = objectProp->GetObjectPropertyValue_InContainer( this );
                
                UWidgetAnimation* widgetAnim = Cast<UWidgetAnimation>(object);
                
                if( widgetAnim != nullptr )
                {
                    // TODO: Filter this functionality out into a TArray or struct like it says below.
                    // The current setup only works if there is one animation.
                    // DO SOMETHING TO STORE OFF THE ANIM PTR HERE!
                    WidgetAnim = widgetAnim;
                    // E.g. add to a TArray of some struct that holds info for each anim
                }
            }
        }
        
        prop = prop->PropertyLinkNext;
    }
}

FSlateBrush UObjectiveComplete::GetFirstCompleteBanner() const
{
    // The brush we will return
    FSlateBrush QuestOneBrush;
    
    if (GetWorld()->GetFirstPlayerController() == nullptr ||
        GetWorld()->GetFirstPlayerController()->GetPawn() == nullptr)
    {
        return FSlateBrush();
    }
    
    ATP_ThirdPersonCharacter* Char = Cast<ATP_ThirdPersonCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    
    if (Char == nullptr)
    {
        return FSlateBrush();
    }
    
    // If quest one complete
    QuestOneBrush.SetResourceObject(ObjCompleteBanner);
    
    return QuestOneBrush;
}

// TODO: This is not being used at the moment due to us using the animation that messes with transparency.
FLinearColor UObjectiveComplete::MakeBannersTransparent() const
{
    return FLinearColor(1.0,1.0,1.0,0.0);
}