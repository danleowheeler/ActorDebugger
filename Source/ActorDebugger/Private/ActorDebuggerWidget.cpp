#include "ActorDebuggerWidget.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Views/SListView.h"
#include "ReflectedPropertyTableRow.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

/*
* Constructs the Slate Widget that holds a header to display the actor's name and class and also a multi column tree view of the properties
*/
void SActorDebuggerWidget::Construct(const FArguments& InArgs)
{
    // define the header row
    TSharedRef<SHeaderRow> HeaderRow 
    = SNew(SHeaderRow)
        + SHeaderRow::Column("PropertyName")
        .DefaultLabel(FText::FromString("Name"))
        .FillWidth(0.1f)
        + SHeaderRow::Column("PropertyValue")
        .DefaultLabel(FText::FromString("Value"))
        .FillWidth(0.9f);

    ChildSlot
    [
	    SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                [
                    SAssignNew(ActorClassNameTextBlock, STextBlock)
                        .ColorAndOpacity(FLinearColor::White)
                ]
                + SHorizontalBox::Slot()
                [
                    SAssignNew(ActorNameTextBlock, STextBlock)
                        .ColorAndOpacity(FLinearColor::Yellow)
                ]
            ]
            + SVerticalBox::Slot()
            .FillHeight(1.0f)
            [
                SNew(SScrollBox)
                + SScrollBox::Slot()
                [
                    SAssignNew(PropertiesView, STreeView<FReflectedActorPropertyPtr>)
                        .TreeItemsSource(&RootItems)
                        .HeaderRow(HeaderRow)
                        .OnGenerateRow_Lambda([](FReflectedActorPropertyPtr Item, const TSharedRef<STableViewBase>& OwnerTree)
                        {
                            return SNew(SReflectedPropertyTableRow, OwnerTree)
                                .ReflectedProperty(Item);
                        })
                        .OnGetChildren_Lambda([](FReflectedActorPropertyPtr Item, TArray<FReflectedActorPropertyPtr>& OutArray)
                        {
                            OutArray = Item->Children;
                        })
                ]
            ]
    ];

    TickerHandle = FTSTicker::GetCoreTicker().AddTicker(
        FTickerDelegate::CreateSP(this, &SActorDebuggerWidget::HandleTicker),
        0.25f
    );
}

/*
* Selects an actor to display in the widget
*/
void SActorDebuggerWidget::SetObject(UObject* Object)
{
    CurrentObject = Object;

    if (IsValid(Object) == false)
    {
        ActorNameTextBlock->SetText(FText::FromString("No Actor Selected"));
        ActorClassNameTextBlock->SetText(FText::FromString("N/A"));
        return;
    }

    ActorNameTextBlock->SetText(FText::FromString(Object->GetName()));
    ActorClassNameTextBlock->SetText(FText::FromString(Object->GetClass()->GetName()));
    
    GatherProperties(RootItems, CurrentObject);

    PropertiesView->RequestListRefresh();
}

/*
* Gathers the properties of the currently selected actor. 
* Places them in the ItemsToFill TArray
* ContainerObject is the UObject that should be reflected
*/
void SActorDebuggerWidget::GatherProperties(TArray<FReflectedActorPropertyPtr>& ItemsToFill, UObject* ContainerObject)
{
    if (!ContainerObject)
    {
        return;
    }

    // Iterate through all properties of the actor
    for (TFieldIterator<FProperty> PropIt(ContainerObject->GetClass()); PropIt; ++PropIt)
    {
        FProperty* Property = *PropIt;

        // Blueprint visible properties are likely to be the most useful ones to display
        if (Property && Property->HasAnyPropertyFlags(CPF_BlueprintVisible))
        {
            auto ReflectedProperty = MakeShared<FReflectedActorProperty>(ContainerObject, Property->GetName() );

            if (UObject* ChildContainerObject = TryGetUObjectValue(ContainerObject, Property); 
                ChildContainerObject && IsUObjectOfInterest(ChildContainerObject))
            {
                GatherProperties(ReflectedProperty->Children, ChildContainerObject);
            }

            ItemsToFill.Add(ReflectedProperty);
        }
    }
}

/*
* If the given property is a FObjectProperty then this will return any UObject that it points to
* If not, returns nullptr
* 'ParentContainer' is the UObject that owns the property
*/
UObject* SActorDebuggerWidget::TryGetUObjectValue(UObject* ParentContainer, FProperty* Property)
{
    auto* ObjectProperty = CastField<FObjectProperty>(Property);

    if (ObjectProperty == nullptr)
    {
        return nullptr;
    }

    TObjectPtr<UObject> ChildObject;
    ObjectProperty->GetValue_InContainer(ParentContainer, &ChildObject);

    return ChildObject;
}

/*
* Returns a boolean that indicates if we want to iterate over the properties of this UObject
*/
bool SActorDebuggerWidget::IsUObjectOfInterest(UObject* Object)
{
    // don't iterate over AActor properties, this opens us up to circular dependencies
    if (Object->IsA(AActor::StaticClass()))
    {
        return false;
    }

    // don't iterate over Asset properties, probably not useful
    if (IsAssetObject(Object))
    {
        return false;
    }

    return true;
}

/*
* Checks for some object flags that are intended to check if an object is an asset type or not
* UE does not have a UAsset class which would be a preferred method but this method appears to work
* correctly
*/
bool SActorDebuggerWidget::IsAssetObject(UObject* Object)
{
    if (!Object)
    {
        return false;
    }
    
    // Assets typically have these flags
    bool bHasAssetFlags = Object->HasAnyFlags(RF_Public | RF_Standalone);
    
    // Assets should not have these flags
    bool bHasNonAssetFlags = Object->HasAnyFlags(RF_Transient | RF_ArchetypeObject | RF_ClassDefaultObject);
    
    return bHasAssetFlags && !bHasNonAssetFlags;
}

/*
* Slate widgets don't typically implement a Tick method so we create a FTSTicker which calls this method
* periodically. It checks if the selected actor has been destroyed and prompts the library to destroy the
* window that owns this widget if so. If not, it will request a refresh of the Tree View which will]
* update the currently rendered widgets
*/
bool SActorDebuggerWidget::HandleTicker(float DeltaTime)
{
    if (IsValid(CurrentObject) == false)
    {
        RequestClose.Broadcast();
    }
    else
    {
        PropertiesView->RequestListRefresh();
    }

    return true;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
