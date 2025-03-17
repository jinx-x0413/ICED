// Fill out your copyright notice in the Description page of Project Settings.


#include "Models/GltfAssetActor.h"
#include "Service/Domain/AssetActor/Private/AssetActorDependency.h"

AGltfAssetActor::AGltfAssetActor()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

	BoxComponent->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	// Collision Color ( Debug )
	BoxComponent->ShapeColor = FColor::Red;

	BoxComponent->SetupAttachment(RootComponent);

	// Debug
	BoxComponent->bHiddenInGame = false;
}

AGltfAssetActor::~AGltfAssetActor()
{
}

void AGltfAssetActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (IsValid(this) && IsRooted())
	{
		RemoveFromRoot();
	}
}



// Asset
void AGltfAssetActor::Initialize(int32 InIndex, UglTFRuntimeAsset* InAsset, FTransform InTransform, const FString& InFilePath, FString InName)
{
	SetName(InName);
	Asset = InAsset;
	bAllowNodeAnimations = true;
	bStaticMeshesAsSkeletal = true;
	bAllowSkeletalAnimations = true;
	bAllowPoseAnimations = true;
	bAllowCameras = true;
	bAllowLights = true;
	bForceSkinnedMeshToRoot = false;
	RootNodeIndex = -1;
	bLoadAllSkeletalAnimations = false;
	//bAutoPlayAnimations = true;
	bAutoPlayAnimations = false; // debug
	bStaticMeshesAsSkeletalOnMorphTargets = true;

	//SetActorTransform(InTransform);
	// Set Location, Scale (Before) Box Extent Set
	InitialTransform = InTransform;
	SetActorLocation(InitialTransform.GetLocation());
	SetActorScale3D(InitialTransform.GetScale3D());

	InitializeAsset();
	SetInitialBoundBoxExtent();

	ResetHierarchyData();
}

void AGltfAssetActor::SetInitialBoundBoxExtent()
{
	if (!BoxComponent)
	{
		return;
	}

	FVector Origin, BoxExtent;
	GetActorBounds(false, Origin, BoxExtent, true);

	BoxComponent->SetWorldLocation(Origin);
	BoxComponent->SetBoxExtent(BoxExtent);

	// No Need to Set Box Component Rotation :: Attached to Actor Rotated
	//BoxComponent->SetWorldRotation(GetActorRotation());


	FVector CurrentRelativeScale, NewRelativeScale;
	CurrentRelativeScale = BoxComponent->GetComponentScale();
	//NewRelativeScale = CurrentRelativeScale * 20.0f;
	NewRelativeScale = CurrentRelativeScale;

	BoxComponent->SetWorldScale3D(NewRelativeScale);
}




// Feature
//// Transform
void AGltfAssetActor::Move(FVector InLocation)
{
	SetActorLocation(InLocation);
}

void AGltfAssetActor::Rotate(FRotator InRotation)
{
	SetActorRotation(InRotation);
}

void AGltfAssetActor::SetScale(FVector InScale)
{
	SetActorScale3D(InScale);
}



//// Hierarchy Data
void AGltfAssetActor::ResetHierarchyData()
{
	HierarchyData.Empty(0);
	GetHierarchyDataRecursive(0); // recursive
}

void AGltfAssetActor::GetHierarchyDataRecursive(int IndentLevel)
{
	FActorHierarchyData NewData;
	NewData.NodeName = GetName(); // 생성한 이름 적용
	NewData.Depth = IndentLevel;
	HierarchyData.Add(NewData);

	USceneComponent* InRootComponent = GetRootComponent();
	if (InRootComponent)
	{
		GetComponentHierarchyRecursive(InRootComponent, IndentLevel + 1);
	}
}

void AGltfAssetActor::GetComponentHierarchyRecursive(USceneComponent* InComponent, int IndentLevel)
{
	if (!InComponent || InComponent->GetFName() == FName("RootScene"))
	{
		return;
	}

	FActorHierarchyData NewData;
	if (Cast<USkeletalMeshComponent>(InComponent))
	{
		NewData.NodeName = InComponent->GetName();
		NewData.Depth = IndentLevel;
		NewData.TargetComponent = InComponent;
		HierarchyData.Add(NewData);

		const TArray<USceneComponent*>& InChildren = InComponent->GetAttachChildren();
		for (USceneComponent* InChild : InChildren)
		{
			GetComponentHierarchyRecursive(InChild, IndentLevel + 1);
		}
	}
	else
	{
		const TArray<USceneComponent*>& InChildren = InComponent->GetAttachChildren();
		for (USceneComponent* InChild : InChildren)
		{
			GetComponentHierarchyRecursive(InChild, IndentLevel);
		}
	}
}

void AGltfAssetActor::SetOutline(bool bIsActivated)
{
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
	GetComponents(SkeletalMeshComponents);

	for (USkeletalMeshComponent* MeshComp : SkeletalMeshComponents)
	{
		if (MeshComp)
		{
			MeshComp->SetRenderCustomDepth(bIsActivated);

			if (bIsActivated)
			{
				MeshComp->CustomDepthStencilValue = 1;
			}
		}
	}
}
