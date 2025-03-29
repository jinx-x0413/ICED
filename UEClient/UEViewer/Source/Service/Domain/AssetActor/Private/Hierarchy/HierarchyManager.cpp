// Fill out your copyright notice in the Description page of Project Settings.


#include "Hierarchy/HierarchyManager.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"

void UHierarchyManager::InitializeHierarchy(AActor* InOwner)
{
    if (!InOwner)
    {
        return;
    }

    /*ResetHierarchyData();
    GetHierarchyDataRecursive(InOwner, 0);*/
}

void UHierarchyManager::ResetHierarchyData()
{
   //HierarchyData.Empty();
}

//void UHierarchyManager::GetHierarchyDataRecursive(AActor* InActor, int32 IndentLevel)
//{
//    if (!InActor)
//    {
//        return;
//    }
//
//    FActorHierarchyData NewData;
//    NewData.NodeName = InActor->GetName();
//    NewData.Depth = IndentLevel;
//    HierarchyData.Add(NewData);
//
//    USceneComponent* InRootComponent = InActor->GetRootComponent();
//    if (InRootComponent)
//    {
//        GetComponentHierarchyRecursive(InRootComponent, IndentLevel + 1);
//    }
//}
//
//void UHierarchyManager::GetComponentHierarchyRecursive(USceneComponent* InComponent, int32 IndentLevel)
//{
//    if (!InComponent || InComponent->GetFName() == FName("RootScene"))
//    {
//        return;
//    }
//
//    FActorHierarchyData NewData;
//    if (Cast<USkeletalMeshComponent>(InComponent))
//    {
//        NewData.NodeName = InComponent->GetName();
//        NewData.Depth = IndentLevel;
//        NewData.TargetComponent = InComponent;
//        HierarchyData.Add(NewData);
//
//        const TArray<USceneComponent*>& InChildren = InComponent->GetAttachChildren();
//        for (USceneComponent* InChild : InChildren)
//        {
//            GetComponentHierarchyRecursive(InChild, IndentLevel + 1);
//        }
//    }
//    else
//    {
//        const TArray<USceneComponent*>& InChildren = InComponent->GetAttachChildren();
//        for (USceneComponent* InChild : InChildren)
//        {
//            GetComponentHierarchyRecursive(InChild, IndentLevel);
//        }
//    }
//}
