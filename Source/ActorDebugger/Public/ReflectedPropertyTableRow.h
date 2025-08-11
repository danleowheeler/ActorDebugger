

/**
 * @file ReflectedPropertyTableRow.h
 * @brief Declares the SReflectedPropertyTableRow class for displaying reflected actor properties in a table row.
 *
 * This file defines the SReflectedPropertyTableRow class, which inherits from SMultiColumnTableRow and is used to
 * represent a single row in a table view for actor properties. The row supports multi-column display and provides
 * widgets for property names, values, and expander arrows for hierarchical property structures.
 *
 * Classes:
 *   - SReflectedPropertyTableRow: Table row widget for displaying a reflected actor property.
 *
 * Key Methods:
 *   - Construct: Initializes the table row with the provided arguments and owner table.
 *   - GenerateWidgetForColumn: Generates the appropriate widget for a given column name.
 *   - GetPropertyNameText: Retrieves the display text for the property name.
 *   - HasChildren: Determines if the property has child properties.
 *   - CreateExpanderArrow: Creates an expander arrow widget for expandable rows.
 *
 * Member Variables:
 *   - PropertyPtr: Pointer to the reflected actor property associated with this row.
 */
#pragma once

#include "Widgets/Views/STableRow.h"
#include "ActorPropertyWidget.h"

class ACTORDEBUGGER_API SReflectedPropertyTableRow : public SMultiColumnTableRow<FReflectedActorPropertyPtr>
{
public:
	SLATE_BEGIN_ARGS(SReflectedPropertyTableRow) {}
		SLATE_ATTRIBUTE(FReflectedActorPropertyPtr, ReflectedProperty)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& OwnerTable);
	
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override;

private:
	FText GetPropertyNameText() const;
	bool HasChildren() const;
	TSharedRef<SWidget> CreateExpanderArrow();

	FReflectedActorPropertyPtr PropertyPtr;
};