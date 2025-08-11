

/**
 * @file ActorDebuggerWidget.h
 * @brief Declares the SActorDebuggerWidget class, a Slate widget for visualizing and interacting with actor properties in Unreal Engine.
 *
 * This widget provides a tree view of reflected actor properties, allowing inspection and debugging of UObject instances.
 * It supports dynamic property gathering, filtering of relevant UObject types, and asset identification.
 * The widget also exposes a multicast delegate for close requests and maintains references to UI elements for displaying actor information.
 *
 * Classes and Types:
 * - SActorDebuggerWidget: Main widget class for actor property debugging.
 * - FReflectedActorPropertyPtr: Smart pointer type for reflected actor properties.
 *
 * Key Methods:
 * - Construct: Initializes the widget with Slate arguments.
 * - SetObject: Sets the UObject to be inspected.
 * - GatherProperties: Populates the property tree with reflected properties.
 * - TryGetUObjectValue: Attempts to retrieve a UObject value from a property.
 * - IsUObjectOfInterest: Determines if a UObject should be displayed.
 * - IsAssetObject: Checks if a UObject is an asset.
 *
 * Delegates:
 * - FOnRequestClose: Multicast delegate for widget close requests.
 *
 * Member Variables:
 * - TickerHandle: Handle for periodic updates.
 * - PropertiesView: Tree view widget for displaying properties.
 * - ActorNameTextBlock, ActorClassNameTextBlock: UI elements for actor information.
 * - CurrentObject: The currently inspected UObject.
 * - RootItems: Array of root property items for the tree view.
 */
#pragma once

#include "Widgets/SCompoundWidget.h"
#include "CoreMinimal.h"
#include "Widgets/Views/STreeView.h"
#include "ActorPropertyWidget.h"

class FProperty;
class AActor;
class STextBlock;

class ACTORDEBUGGER_API SActorDebuggerWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SActorDebuggerWidget)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void SetObject(UObject* Object);
	DECLARE_MULTICAST_DELEGATE(FOnRequestClose);

	FOnRequestClose RequestClose;
private:
	bool HandleTicker(float DeltaTime);
	void GatherProperties(TArray<FReflectedActorPropertyPtr>& ItemsToFill, UObject* ContainerObject);
    UObject* TryGetUObjectValue(UObject* ParentContainer, FProperty* Property);
	bool IsUObjectOfInterest(UObject* Object);
	bool IsAssetObject(UObject* Object);	


	FTSTicker::FDelegateHandle TickerHandle;
	TSharedPtr<STreeView<FReflectedActorPropertyPtr>> PropertiesView;
	TSharedPtr<STextBlock> ActorNameTextBlock;
	TSharedPtr<STextBlock> ActorClassNameTextBlock;
	TObjectPtr<UObject> CurrentObject{nullptr};
	TArray<FReflectedActorPropertyPtr> RootItems;
};
