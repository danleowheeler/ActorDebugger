/**
 * @file ActorDebugger.h
 * @brief Declares the FActorDebuggerModule class, which implements the Actor Debugger plugin module for Unreal Engine.
 *
 * This header defines the interface for the Actor Debugger module, providing startup and shutdown functionality
 * for the plugin. The module is intended to assist with debugging actors within the Unreal Engine editor.
 *
 * Copyright Dan Wheeler. All Rights Reserved.
 */

#pragma once

#include "Modules/ModuleManager.h"

class FActorDebuggerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
