// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandStack.h"
#include "CommandDependency.h"

// construct
UCommandStack::UCommandStack()
{
}

UCommandStack::~UCommandStack()
{
	if (IsValid(this) && IsRooted())
	{
		if (IsValid(History) && History->IsRooted())
		{
			for (auto& Command : History->GetCommands())
			{
				if (IsValid(Command) && Command->IsRooted())
				{
					Command->RemoveFromRoot();
					Command->MarkAsGarbage();
					Command = nullptr;
				}
			}

			History->RemoveFromRoot();
			History->MarkAsGarbage();
			History = nullptr;
		}

		RemoveFromRoot();
		MarkAsGarbage();
	}
}




// feature
UCommandHistory* UCommandStack::GetHistory()
{
	if(!History)
	{
		History = NewObject<UCommandHistory>();
		History->AddToRoot();
	}
	return History;
}

void UCommandStack::AddCommandToHistory(UCommandBase* InCommand)
{
	if (IsValid(History))
	{
		History->AddCommandToHistory(InCommand);
	}
}

void UCommandStack::ExecuteCommand(UCommandBase* InCommand)
{
	if (InCommand)
	{
		InCommand->Execute();
	}
}

void UCommandStack::Undo()
{
	if (GetHistory() && GetHistory()->GetCommands().Num() > 0)
	{
		GetHistory()->PopCommand()->Undo();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Last Command is not valid at CommandStack"));
	}
}
