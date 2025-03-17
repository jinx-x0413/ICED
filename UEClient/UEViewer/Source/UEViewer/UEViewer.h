// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// 주 모듈 클래스 정의
class FUEViewer: public FDefaultGameModuleImpl
{
public:
    // 주 모듈을 초기화할 때 필요한 작업
    virtual void StartupModule() override;

    // 주 모듈이 종료될 때 필요한 작업
    virtual void ShutdownModule() override;

    // 루트 모듈에 해당하는 기능
    void InitializeRootModule();

private:
    // 서브 모듈을 등록하는 함수
    void RegisterSubModule(FString ModuleName);

    // 서브 모듈을 셧다운하고 언로드하는 함수
    void ShutdownAndUnloadModule(FString ModuleName);

};
