/**
 * @file ActorPropertyWidget.h
 * @brief Declares the SActorPropertyWidget class and supporting structures for displaying and interacting with reflected actor properties in the Actor Debugger plugin.
 *
 * This file defines the FReflectedActorProperty structure, which represents a property of an actor, including its name, value, and child properties.
 * It also declares the SActorPropertyWidget Slate widget, which is responsible for rendering and managing the UI for actor properties.
 *
 * Classes and Structures:
 * - FReflectedActorProperty: Holds information about a reflected property, including its owning UObject, property name, and any child properties.
 * - SActorPropertyWidget: A Slate widget for displaying actor properties in a hierarchical view.
 *
 * Usage:
 * Include this header in modules that need to display or interact with actor properties in the Actor Debugger plugin.
 */
#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/ITableRow.h"

class FProperty;

struct FReflectedActorProperty;

typedef TSharedPtr<FReflectedActorProperty> FReflectedActorPropertyPtr;

struct FReflectedActorProperty
{
	UObject* Object {nullptr};
	FString PropertyName;
    TArray<FReflectedActorPropertyPtr> Children;

    bool IsValid() const
    {
        return ::IsValid(Object) && PropertyName.Len() > 0;;
    }
};


class ACTORDEBUGGER_API SActorPropertyWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SActorPropertyWidget){}
        SLATE_ATTRIBUTE(FReflectedActorPropertyPtr, ReflectedProperty)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
private:
    FText PropertyValueToText(const FReflectedActorPropertyPtr& Property);
};
