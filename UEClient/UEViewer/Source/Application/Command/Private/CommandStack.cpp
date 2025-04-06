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
	if (!History)
	{
		History = GetHistory();
	}

	if (IsValid(History))
	{
		History->AddCommandToHistory(InCommand);
	}
}

UCommandBase* UCommandStack::GetLastCommand()
{
	if (!History)
	{
		History = GetHistory();
	}

	if (IsValid(History) && History->GetCommands().Num() > 0)
	{
		return History->GetCommands()[History->GetCommands().Num() - 1];
	}
	return nullptr;
}

UCommandBase* UCommandStack::PopLastCommand()
{
	if (!History)
	{
		History = GetHistory();
	}

	if (IsValid(History))
	{
		return History->PopCommand();
	}
	
	return nullptr;
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
		if (UCommandBase* PopCommand = GetHistory()->PopCommand())
		{
			PopCommand->Undo();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Last Command at Command History"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Last Command is not valid at CommandStack"));
	}
}




// factory
UCommandBase* UCommandStack::CreateCommand(ECommandType InType)
{
	UCommandBase* NewCommand;
	switch (InType)
	{
	case ECommandType::ASSETACTOR_TRANSFORM :
		NewCommand = NewObject<USetAssetActorTransformCommand>();
		break;
		
	default:
		return nullptr;
		break;
	}
	
	NewCommand->AddToRoot();
	AddCommandToHistory(NewCommand);
	return NewCommand;
}
