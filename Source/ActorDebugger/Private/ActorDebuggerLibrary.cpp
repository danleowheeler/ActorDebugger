// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorDebuggerLibrary.h"

#include "ActorDebuggerWidget.h"
#include "Widgets/SWindow.h"

TSharedPtr<SWindow> UActorDebuggerLibrary::Window;

/*
 * Selects the specified actor in the Actor Debugger window.
 *
 * If the debugger window is already open, updates its content to display the given actor.
 * Otherwise, creates a new debugger window, sets its content to display the actor, and adds it to the Slate application.
 * Also binds a lambda to the widget's RequestClose event to properly destroy and reset the window when closed.
 *
 * @param ActorToSelect The actor to be selected and displayed in the debugger window.
 * @return true if the actor was successfully selected and the window updated or created; false otherwise.
 */
bool UActorDebuggerLibrary::SelectActor(AActor* ActorToSelect)
{
    if (Window)
    {
        TSharedRef<SActorDebuggerWidget> DebuggerWidget = StaticCastSharedRef<SActorDebuggerWidget>(Window->GetContent());
        DebuggerWidget.Get().SetObject(ActorToSelect);
        return true;
    }

    TSharedPtr<SActorDebuggerWidget> DebuggerWidget;

    Window = SNew(SWindow).ClientSize(FVector2D{250,250}).IsTopmostWindow(true)
    [
        SAssignNew(DebuggerWidget, SActorDebuggerWidget)
    ];

	FSlateApplication::Get().AddWindow(Window.ToSharedRef());
    DebuggerWidget->SetObject(ActorToSelect);
    DebuggerWidget->RequestClose.AddLambda(
        []
        { 
	        FSlateApplication::Get().RequestDestroyWindow(Window.ToSharedRef());
            Window.Reset();
        });
    return true;
}