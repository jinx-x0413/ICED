// Fill out your copyright notice in the Description page of Project Settings.


#include "Table/Fields/TableFieldCombobox.h"
#include "../TableDependency.h"

void UTableFieldCombobox::NativeConstruct()
{
	Super::NativeConstruct();

	TransformDirectionArray = {
		TEXT("None")
		, TEXT("Auto")
		, TEXT("Up")
		, TEXT("Down")
		, TEXT("Left")
		, TEXT("Right")
	};

	ExecConstruct();
}

void UTableFieldCombobox::NativeDestruct()
{
	Super::NativeDestruct();

	TransformDirectionArray.Empty();
}
