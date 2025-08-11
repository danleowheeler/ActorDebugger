#include "ActorPropertyWidget.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Layout/SBox.h"
#include "GameFramework/Actor.h"
#include "UObject/UnrealType.h"
#include "ActorDebuggerWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

/*
* Constructs the Slate widget
*/
void SActorPropertyWidget::Construct(const FArguments& InArgs)
{
    ChildSlot
    [
        SNew(STextBlock).Text(PropertyValueToText(InArgs._ReflectedProperty.Get()))
    ];
}

/*
* Converts the reflected property value to an FText. The method used seems to do a decent job of generically
* converting the value to a human readable string
*/
FText SActorPropertyWidget::PropertyValueToText(const FReflectedActorPropertyPtr& PropertyPtr)
{
    if (PropertyPtr == nullptr || PropertyPtr->IsValid() == false)
    {
        return FText::FromString("Invalid Property");
    }

    FProperty* Property {nullptr};
    Property = PropertyPtr->Object->GetClass()->FindPropertyByName(FName(*PropertyPtr->PropertyName));

    FText PropertyValue;
    if (Property == nullptr)
    {
        return FText::FromString("Property not found");
    }   

    FString ValueString;
    Property->ExportText_InContainer(0, ValueString, PropertyPtr->Object, PropertyPtr->Object, PropertyPtr->Object, PPF_BlueprintDebugView);
    return FText::FromString(ValueString);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
