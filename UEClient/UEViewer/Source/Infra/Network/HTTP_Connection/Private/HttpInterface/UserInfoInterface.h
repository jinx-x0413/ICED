// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HttpInterface.h"

#include "UserInfoInterface.generated.h"

/**
 * 
 */
UCLASS()
class UUserInfoInterface : public UObject, public IHttpInterface
{
	GENERATED_BODY()
	
public:
	virtual void Start() override;

};
