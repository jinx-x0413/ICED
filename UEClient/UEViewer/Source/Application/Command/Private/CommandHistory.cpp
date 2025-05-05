#include "CommandHistory.h"
#include "CommandDependency.h"

// construct
UCommandHistory::UCommandHistory()
{
}

UCommandHistory::~UCommandHistory()
{
}

void UCommandHistory::BeginDestroy()
{
    Shutdown();

    Super::BeginDestroy();
}

void UCommandHistory::Shutdown()
{
    if (!Commands.IsEmpty())
    {
        for (auto& Command : Commands)
        {
            if (IsValid(Command) && Command->IsRooted())
            {
                Command->RemoveFromRoot();
                Command->MarkAsGarbage();
                Command = nullptr;
            }
        }

        Commands.Empty();
    }
}





void UCommandHistory::AddCommandToHistory(UCommandBase* InCommand)
{
    // Add the command to the history list
    if (IsValid(InCommand))
    {
        Commands.Add(InCommand);
    }
}

UCommandBase* UCommandHistory::PopCommand()
{
    // Check if there are commands in history
    if (GetCommands().Num() > 0)
    {
        // Get the last command from history
        UCommandBase* LastCommand = Commands.Pop();

        // Check validity and rooted status before removal and destruction
        if (IsValid(LastCommand) && LastCommand->IsRooted())
        {
            // Safely remove it from the root and begin its destruction
            LastCommand->RemoveFromRoot();
            LastCommand->MarkAsGarbage();  // Begin destruction
        }

        return LastCommand;
    }
    return nullptr;  // Return nullptr if no commands exist
}
