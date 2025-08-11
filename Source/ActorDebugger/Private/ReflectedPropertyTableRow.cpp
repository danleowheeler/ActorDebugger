#include "ReflectedPropertyTableRow.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Views/STreeView.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

/*
* Constructs the main widget
*/
void SReflectedPropertyTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& OwnerTable)
{
    PropertyPtr = InArgs._ReflectedProperty.Get();

    SMultiColumnTableRow<FReflectedActorPropertyPtr>::Construct(
        FSuperRowType::FArguments()
        .ShowWires(true) // this doesn't seem to be working. TODO
        .Padding(FMargin(0,2)),
        OwnerTable
    );
}

/*
* Constructs the correct widget for the given table column
*/
TSharedRef<SWidget> SReflectedPropertyTableRow::GenerateWidgetForColumn(const FName& ColumnName)
{
    if (ColumnName == TEXT("PropertyName"))
    {
        return SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .AutoWidth()
            .Padding(FMargin(0, 0, 4, 0))
            [
                CreateExpanderArrow()
            ]
            + SHorizontalBox::Slot()
            .FillWidth(1.0f)
            [
                SNew(STextBlock)
                    .ColorAndOpacity(FLinearColor(0.75f, 0.75f, 0.75f))
                    .Text(this, &SReflectedPropertyTableRow::GetPropertyNameText)
            ];
    }
    else if (ColumnName == TEXT("PropertyValue"))
    {
        return SNew(SActorPropertyWidget)
            .ReflectedProperty(PropertyPtr);
    }
    
    return SNullWidget::NullWidget;
}

/*
* Safely gets the name of the selected property
*/
FText SReflectedPropertyTableRow::GetPropertyNameText() const
{
    if (PropertyPtr == nullptr || PropertyPtr->IsValid() == false)
    {
        return FText::FromString("Invalid Property");
    }
    
    return FText::FromString(PropertyPtr->PropertyName);
}

/*
* This may or may not be needed for the expander arrow to be displayed if the property has child properties
*/
bool SReflectedPropertyTableRow::HasChildren() const
{
    // TODO: check if this method is needed
    return PropertyPtr.IsValid() && PropertyPtr->Children.Num() > 0;
}

/*
* Constructs the Expander Arrow widget
*/
TSharedRef<SWidget> SReflectedPropertyTableRow::CreateExpanderArrow()
{
    if (!HasChildren())
    {
        return SNew(SBox)
            .WidthOverride(16)
            .HeightOverride(16);
    }

    return SNew(SExpanderArrow, SharedThis(this))
        .IndentAmount(12);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION