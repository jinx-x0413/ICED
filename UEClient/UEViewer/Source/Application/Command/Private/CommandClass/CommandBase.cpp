#include "CommandClass/CommandBase.h"

void UCommandBase::Initialize(FCommandData& InData)
{
    // Initialize target data
    TargetData = InData;
}

void UCommandBase::Execute()
{
    // Implement execution logic
}

void UCommandBase::Undo()
{
    // Implement undo logic
}

void UCommandBase::BeginDestroy()
{
    Super::BeginDestroy();

}
