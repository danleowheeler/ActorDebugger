

/**
 * @file ActorDebuggerLibrary.h
 * @brief Declares the UActorDebuggerLibrary class, providing static utility functions for actor debugging in Unreal Engine.
 *
 * This header defines the UActorDebuggerLibrary class, which extends UBlueprintFunctionLibrary to expose
 * debugging-related functionality to Blueprints. It includes methods for selecting actors and managing
 * a shared window instance for debugging purposes.
 *
 * @copyright Copyright (c) Dan Wheeler. All rights reserved.
 */
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ActorDebuggerLibrary.generated.h"

UCLASS()
class ACTORDEBUGGER_API UActorDebuggerLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static TSharedPtr<SWindow> Window;
	UFUNCTION(BlueprintCallable, Category = "Actor Debugger")
	static bool SelectActor(AActor* ActorToSelect);

};
