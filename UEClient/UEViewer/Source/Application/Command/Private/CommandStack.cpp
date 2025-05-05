// Fill out your copyright notice in the Description page of Project Settings.

#include "CommandStack.h"
#include "CommandDependency.h"

// construct
UCommandStack::UCommandStack()
{
}

UCommandStack::~UCommandStack()
{
	// Destructor에서 조건부 파괴 로직을 처리하지 않고, 
	// BeginDestroy에서 처리하도록 해야 함
}

// BeginDestroy에서 GC 관련 로직 처리
void UCommandStack::BeginDestroy()
{
	if (IsPendingKill()) return;  // 이미 파괴 중인 객체는 건너뜀

	// History 파괴 처리
	if (IsValid(History) && History->IsRooted())
	{
		History->RemoveFromRoot();
		History->MarkAsGarbage();  // History 파괴 시작
		History = nullptr;
	}
	
	Super::BeginDestroy();  // 부모 클래스의 BeginDestroy 호출
}





// feature
UCommandHistory* UCommandStack::GetHistory()
{
	if(!History)
	{
		History = NewObject<UCommandHistory>(GetTransientPackage());
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
