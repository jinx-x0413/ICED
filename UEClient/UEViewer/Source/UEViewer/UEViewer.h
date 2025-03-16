// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// �� ��� Ŭ���� ����
class FUEViewer: public FDefaultGameModuleImpl
{
public:
    // �� ����� �ʱ�ȭ�� �� �ʿ��� �۾�
    virtual void StartupModule() override;

    // �� ����� ����� �� �ʿ��� �۾�
    virtual void ShutdownModule() override;

    // ��Ʈ ��⿡ �ش��ϴ� ���
    void InitializeRootModule();

private:
    // ���� ����� ����ϴ� �Լ�
    void RegisterSubModule(FString ModuleName);

    // ���� ����� �˴ٿ��ϰ� ��ε��ϴ� �Լ�
    void ShutdownAndUnloadModule(FString ModuleName);

};
