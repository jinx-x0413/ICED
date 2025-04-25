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
    // Ensure the base class BeginDestroy is called
    Super::BeginDestroy();

    // Any specific cleanup or resource release can be done here
    // If you need to remove from root or handle specific cases, do so here
    if (IsRooted())
    {
        RemoveFromRoot();
        ConditionalBeginDestroy(); // Begin destruction if necessary
    }
}
