// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LogManager.generated.h"

/**
 * 
 */
UCLASS()
class LOG_API ULogManager : public UObject
{
	GENERATED_BODY()
	
    // construct / singleton
public:
    ULogManager();
    virtual ~ULogManager();
    static ULogManager* GetLogManager();
    static ULogManager* Instance;




    // feature
public:
    void ParseAuthToken();          // 실행 인자로부터 토큰 파싱
    FString GetToken() const;       // 현재 토큰 반환
    bool IsLoggedIn() const;        // 로그인 여부 판단

private:
    FString AuthToken;
};
