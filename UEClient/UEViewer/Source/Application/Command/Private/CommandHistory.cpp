// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandHistory.h"
#include "CommandDependency.h"

void UCommandHistory::AddCommandToHistory(UCommandBase* InCommand)
{
	GetCommands().Add(InCommand);
}

UCommandBase* UCommandHistory::PopCommand()
{
	if (GetCommands().Num() > 0)
	{
		UCommandBase* LastCommand = GetCommands().Pop();
		if (IsValid(LastCommand) && LastCommand->IsRooted())
		{
			LastCommand->RemoveFromRoot();
			LastCommand->MarkAsGarbage();
		}
		return LastCommand;
	}
	return nullptr;
}
