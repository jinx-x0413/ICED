// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandClass/SetAssetActorTransformCommand.h"

void USetAssetActorTransformCommand::Execute()
{
}

void USetAssetActorTransformCommand::Undo()
{
	TargetData.TargetActor->SetActorTransform(TargetData.OldTransform);
}
