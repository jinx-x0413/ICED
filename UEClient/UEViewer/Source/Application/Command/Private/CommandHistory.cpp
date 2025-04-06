// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandHistory.h"
#include "CommandDependency.h"


void UCommandHistory::AddCommandToHistory(UCommandBase* InCommand)
{
	Commands.Add(InCommand);
}

UCommandBase* UCommandHistory::PopCommand()
{
	if (GetCommands().Num() > 0)
	{
		UCommandBase* LastCommand = Commands.Pop();
		if (IsValid(LastCommand) && LastCommand->IsRooted())
		{
			LastCommand->RemoveFromRoot();
			LastCommand->MarkAsGarbage();
		}
		return LastCommand;
	}
	return nullptr;
}
