// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandClass/CommandBase.h"

void UCommandBase::Initialize(FCommandData& InData)
{
	TargetData = InData;
}

void UCommandBase::Execute()
{
}

void UCommandBase::Undo()
{
	
}
