// Fill out your copyright notice in the Description page of Project Settings.


#include "CorePawn.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"

ACorePawn::ACorePawn()
{
	// Initial Settings
	bIsPanning = false;
	InitialX = 0.0f;
	InitialY = 0.0f;
	PanSpeed = 0.50f; // 0.25 to 0.5
	LocationX = 0.0f;
	LocationY = 0.0f;
	RotationSpeed = 2.0f;
	bIsHold = false;

	ClampValue = 0.0f;
	WheelSpeed = -1000.0f; // -700 to -500 to -1000
	ZoomScaleFactor = 0.25f; // If you want to adjust the zoom speed, tweak this value. Decrease it for a slower zoom, or increase it for a faster zoom
	Min = -2000.0;
	Max = 5000.0f;
	InterpSpeed = 5.0f;

	// Create Spring Arma And Camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	//SpringArm->SetupAttachment(GetMeshComponent());
	Camera->SetupAttachment(SpringArm);

	// SpringArm Settings
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 10;
	SpringArm->CameraRotationLagSpeed = 10;
	SpringArm->bDoCollisionTest = false;

	CamGoal = SpringArm->TargetArmLength;

	//SelectedActor = nullptr;
}

void ACorePawn::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC != nullptr)
	{
		PC->SetIgnoreMoveInput(true);
		PC->SetShowMouseCursor(true);
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" GetFirstPlayerController() Returns null "));
	}

	USphereComponent* DefaultCollision = Cast<USphereComponent>(GetComponentByClass(USphereComponent::StaticClass()));
	if (DefaultCollision)
	{
		DefaultCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		//UE_LOG(LogTemp, Warning, TEXT(" CollisionDisabled "));
		DefaultCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
		DefaultCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
		DefaultCollision->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
		DefaultCollision->SetCollisionResponseToChannel(ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
		DefaultCollision->SetCollisionResponseToChannel(ECC_Vehicle, ECollisionResponse::ECR_Overlap);
		DefaultCollision->SetCollisionResponseToChannel(ECC_Destructible, ECollisionResponse::ECR_Overlap);
	}


	if (!TransformerPawn)
	{
		FVector SpawnLocation = FVector(0.0f, 0.0f, 0.0f);
		FRotator SpawnRotator = FRotator(0.0f, 0.0f, 0.0f);

		TransformerPawn = Cast<ATransformerPawn>(GetWorld()->SpawnActor(ATransformerPawn::StaticClass()));
	}
	if (TransformerPawn)
	{
		TransformerPawn->mPC = Cast<APlayerController>(GetController());
		InitializeGizmoDelegate();
	}

}

void ACorePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Right Mouse Button Pressed
	PlayerInputComponent->BindAction(("Panning"), IE_Pressed, this, &ACorePawn::StartCamPanning);
	PlayerInputComponent->BindAction(("Panning"), IE_Released, this, &ACorePawn::EndCamPanning);
	// Left Mouse Button Pressed
	PlayerInputComponent->BindAction(("RotatingCam"), IE_Pressed, this, &ACorePawn::StartCamRotating);
	PlayerInputComponent->BindAction(("RotatingCam"), IE_Released, this, &ACorePawn::EndCamRotating);

	PlayerInputComponent->BindAxis(("MouseX"), this, &ACorePawn::GetMouseX);
	PlayerInputComponent->BindAxis(("MouseY"), this, &ACorePawn::GetMouseY);
	PlayerInputComponent->BindAxis(("Zoom"), this, &ACorePawn::Zoom);

}

void ACorePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPanning)
	{
		PanCamera();
	}

}

void ACorePawn::StartCamPanning()
{
	//UE_LOG(LogTemp, Warning, TEXT(" Panning True "));

	bIsPanning = true;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();


	if (PC)
	{
		PC->GetMousePosition(InitialX, InitialY);
		//UE_LOG(LogTemp, Warning, TEXT("Mouse X: %f, Mouse Y: %f"), InitialX, InitialY);
	}
	else if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT(" Failed to obtain PlayerController reference "));
	}

}

void ACorePawn::EndCamPanning()
{
	bIsPanning = false;
}

void ACorePawn::PanCamera()
{
	UGameplayStatics::GetPlayerController(this, 0)->GetMousePosition(LocationX, LocationY);
	CamY = GetActorRightVector() * ((InitialX - LocationX) * PanSpeed);
	CamZ = GetActorUpVector() * ((InitialY - LocationY) * PanSpeed);
	AActor::SetActorLocation(((CamY * 1) + (CamZ * (-1)) + GetActorLocation()));
	UGameplayStatics::GetPlayerController(this, 0)->GetMousePosition(InitialX, InitialY);
}

void ACorePawn::StartCamRotating()
{
	bIsHold = true;
}

void ACorePawn::EndCamRotating()
{
	bIsHold = false;
}

void ACorePawn::GetMouseX(float AxisValue)
{
	if (bIsHold && !TransformerPawn->bIsActivatingGizmo)
	{
		AddActorWorldRotation(UKismetMathLibrary::MakeRotator(0.0f, 0.0f, (AxisValue * RotationSpeed)));
	}
}

void ACorePawn::GetMouseY(float AxisValue)
{
	if (bIsHold && !TransformerPawn->bIsActivatingGizmo)
	{
		AddActorLocalRotation(UKismetMathLibrary::MakeRotator(0.0f, (AxisValue * RotationSpeed), 0.0f));
	}
}

void ACorePawn::Zoom(float NewAxisValue)
{
	ClampValue = (NewAxisValue * WheelSpeed * ZoomScaleFactor) + CamGoal;

	CamGoal = FMath::Clamp(ClampValue, Min, Max);
	if (!FMath::IsNearlyEqual(CamGoal, SpringArm->TargetArmLength))
	{
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, CamGoal, UGameplayStatics::GetWorldDeltaSeconds(this), InterpSpeed);
	}
	else
	{
		CamGoal = SpringArm->TargetArmLength;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Final Clamp Value %f"),ClampValue);
}

void ACorePawn::MoveForward(float Value)
{
	if (Controller && Value != 0.0f)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		FRotator CameraRotation;
		if (PC)
		{
			CameraRotation = PC->PlayerCameraManager->GetCameraRotation();
		}
		else
		{
			CameraRotation = Controller->GetControlRotation();
		}

		FRotator YawRotation(0, CameraRotation.Yaw, 0);
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void ACorePawn::MoveRight(float Value)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (Controller && Value != 0.0f)
	{
		FRotator CameraRotation;
		if (PC)
		{
			CameraRotation = PC->PlayerCameraManager->GetCameraRotation();
		}
		else
		{
			CameraRotation = Controller->GetControlRotation();
		}

		FRotator YawRotation(0, CameraRotation.Yaw, 0);
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

TArray<USkeletalMeshComponent*> ACorePawn::GetSkeletalMeshComponents(AActor* TargetActor)
{
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
	if (TargetActor)
	{
		TargetActor->GetComponents(SkeletalMeshComponents);
	}

	return SkeletalMeshComponents;
}





// control object

void ACorePawn::SelectActor(AActor* InActor)
{

	if (!InActor || TransformerPawn->bIsModifying)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No Actor Selected at CorePawn SelectActor"));
		return;
	}
	if (TransformerPawn->bIsActivatingGizmo)
	{
		return;
	}


	if (InActor == TransformerPawn->CurrentSelectActor) // 이미 선택 중인 액터
	{

		// 1. set current select actor
		TransformerPawn->SetCurrentSelectActor(InActor);

		// 2. gizmo
		if (!TransformerPawn->bIsGizmoDisplayed)
		{
			ShowGizmo();
		}

		// TODO : delegate callback (AssetActorManager)
		// 3. set outline
		//		전체 해제
		//		선택 적용

	}
	else // 새롭게 선택한 액터
	{
		// 1. set current select actor
		TransformerPawn->SetCurrentSelectActor(InActor);

		// 2. gizmo
		if (!TransformerPawn->bIsPreviewing)
		{
			ShowGizmo();
		}

		// TODO : delegate callback (AssetActorManager)
		// 3. set outline
		//		선택 적용
		//SetGltfOutline(InActor, true);
	}

	//UEventDispatcher::GetEventDispatcher()->SetSelectedActorOutline(); // TODO : delegate callback (AssetActorManager)
	TransformerPawn->OnActiveSelect.Broadcast();
}

void ACorePawn::ClearSelectActor()
{


	if (TransformerPawn->CurrentSelectActor)
	{
		SetGltfOutline(TransformerPawn->CurrentSelectActor, false);
		TransformerPawn->CurrentSelectActor = nullptr;
		TransformerPawn->DestroyAndClearGizmo();
		//TransformerPawn->OnDeactiveSelect.Broadcast();
	}


	//SelectedActor = nullptr;

}

void ACorePawn::SetGltfOutline(AActor* TargetActor, bool bEnable)
{
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents = GetSkeletalMeshComponents(TargetActor);
	for (USkeletalMeshComponent* MeshComp : SkeletalMeshComponents)
	{
		if (MeshComp)
		{
			MeshComp->SetRenderCustomDepth(bEnable);

			if (bEnable)
			{
				MeshComp->CustomDepthStencilValue = 1;
			}
		}
	}
}

void ACorePawn::SetComponentOutline(USkeletalMeshComponent* TargetComponent, bool bEnable)
{
	if (!TargetComponent)
	{
		return;
	}

	TargetComponent->SetRenderCustomDepth(bEnable);

	if (bEnable)
	{
		TargetComponent->CustomDepthStencilValue = 1;
	}
}

void ACorePawn::ShowGizmo()
{
	TransformerPawn->bIsGizmoDisplayed = true;
	TransformerPawn->SetTransformationType(ETransformationType::TT_Translation);
	TransformerPawn->ForceCreateGizmo();
}






void ACorePawn::InitializeGizmoDelegate()
{
	if (TransformerPawn)
	{

		/*TransformerPawn->OnGizmoPressed.AddDynamic(UEventDispatcher::GetEventDispatcher(), &UEventDispatcher::StartGltfAssetActorTransform);
		TransformerPawn->OnGizmoReleased.AddDynamic(UEventDispatcher::GetEventDispatcher(), &UEventDispatcher::UpdateGltfAssetActorTransform);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Gizmo Delegate Initialized at CorePawn"));*/
	}
}