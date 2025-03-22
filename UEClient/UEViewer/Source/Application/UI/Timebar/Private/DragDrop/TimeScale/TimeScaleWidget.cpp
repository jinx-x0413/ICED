// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeScaleWidget.h"
#include "../TimebarDragDropDependency.h"
#include "../../Player/PlayerDependency.h"

// construct
void UTimeScaleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsRulerDrawn = false;

	if (!UTimebarPlayer::GetTimebarPlayer()->OnStarted.IsAlreadyBound(this, &UTimeScaleWidget::DrawRuler))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnStarted.AddDynamic(this, &UTimeScaleWidget::DrawRuler);
	}

	if (!UTimebarPlayer::GetTimebarPlayer()->OnCurrentTimeChanged.IsAlreadyBound(this, &UTimeScaleWidget::FinishPlay))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnCurrentTimeChanged.AddDynamic(this, &UTimeScaleWidget::FinishPlay);
	}

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
	

	CurrentRulerSpacing = Variables::InitialRulerSpacing;
	CurrentZoomScale = Variables::InitialZoomScale;
	//DrawRuler();
}

void UTimeScaleWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (!UTimebarPlayer::GetTimebarPlayer()->OnStarted.IsAlreadyBound(this, &UTimeScaleWidget::DrawRuler))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnStarted.RemoveDynamic(this, &UTimeScaleWidget::DrawRuler);
	}

	if (UTimebarPlayer::GetTimebarPlayer()->OnCurrentTimeChanged.IsAlreadyBound(this, &UTimeScaleWidget::FinishPlay))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnCurrentTimeChanged.RemoveDynamic(this, &UTimeScaleWidget::FinishPlay);
	}
}

void UTimeScaleWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (bIsRulerDrawn)
	{
		return;
	}

	Super::NativeTick(MyGeometry, InDeltaTime);

	// GetCachedGeometry()가 유효한지 확인하고, 한번만 DrawRuler를 호출
	
	if (GetCachedGeometry().GetLocalSize().X > 0)
	{
		DrawRuler();  // 눈금자 그리기
		bIsRulerDrawn = true;  // 한 번만 그려지도록 설정
	}
}





// drag
FReply UTimeScaleWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		// click
		FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
		FVector2D LocalMousePosition = InGeometry.AbsoluteToLocal(MousePosition); // Local space로 변환

		/*UTimebarPlayer::GetTimebarPlayer()->CurrentTime = LocalMousePosition.X / (Variables::ConvertedValue* Variables::ScaleMultiplier);
		UTimebarPlayer::GetTimebarPlayer()->OnCurrentTimeChanged.Broadcast(LocalMousePosition.X/ (Variables::ConvertedValue* Variables::ScaleMultiplier));*/
		UTimebarPlayer::GetTimebarPlayer()->SetCurrentTime(LocalMousePosition.X / (Variables::ConvertedValue * Variables::ScaleMultiplier));

		// drag
		bIsDragging = true;

		return FReply::Handled();
	}

	return FReply::Unhandled();
}



FReply UTimeScaleWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsDragging)
	{
		// 마우스 위치 업데이트
		FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
		FVector2D LocalMousePosition = InGeometry.AbsoluteToLocal(MousePosition);

		// Time 업데이트
		/*UTimebarPlayer::GetTimebarPlayer()->CurrentTime = LocalMousePosition.X / (Variables::ConvertedValue* Variables::ScaleMultiplier);
		UTimebarPlayer::GetTimebarPlayer()->OnCurrentTimeChanged.Broadcast(LocalMousePosition.X / (Variables::ConvertedValue* Variables::ScaleMultiplier));*/
		UTimebarPlayer::GetTimebarPlayer()->SetCurrentTime(LocalMousePosition.X / (Variables::ConvertedValue * Variables::ScaleMultiplier));

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply UTimeScaleWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsDragging = false;
	return FReply::Handled();
}

void UTimeScaleWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	bIsDragging = false;
}





// play
void UTimeScaleWidget::SetMaxScale()
{
	MaxScale = GetCachedGeometry().GetLocalSize().X;
}

void UTimeScaleWidget::FinishPlay(float InCurrentTime)
{
	if (InCurrentTime * Variables::ConvertedValue* Variables::ScaleMultiplier >= MaxScale && GetCachedGeometry().GetLocalSize().X >= MaxScale)
	{
		UTimebarPlayer::GetTimebarPlayer()->OnFinished.Broadcast();
	}
}




//// Scale
void UTimeScaleWidget::DrawRuler()
{
	// 최대 스케일 초기화
	SetMaxScale();

	// 눈금 초기화
	if (!RulerTickArray.IsEmpty())
	{
		for (auto& Tick : RulerTickArray)
		{
			Tick->RemoveFromParent();
		}
		RulerTickArray.Empty();
	}

	// 현재 캔버스를 가져옵니다.
	UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(GetRootWidget());
	
	if (CanvasPanel)
	{
		// 눈금자에 그릴 각 눈금의 크기
		float SpacingAdjusted = CurrentRulerSpacing * CurrentZoomScale;

		// 눈금자 그리기 (간격에 맞는 선 그리기)
		for (float i = 0; i < MaxScale; i += SpacingAdjusted)
		{
			// 각 눈금의 위치에 맞는 Border 또는 Line을 그립니다.
			UBorder* NewRulerTick = NewObject<UBorder>(this);

			// 기본 Border 디자인
			NewRulerTick->SetBrushColor(FLinearColor::White);
			NewRulerTick->SetPadding(FMargin(0));

			// 캔버스에 추가
			CanvasPanel->AddChild(NewRulerTick);
			RulerTickArray.Add(NewRulerTick);

			// Canvas에 배치
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(NewRulerTick->Slot);
			if (CanvasSlot)
			{
				CanvasSlot->SetAnchors(FAnchors(0.0f, 1.0f, 0.0f, 1.0f));  // X=0.5f (가운데), Y=1.0f (하단)
				CanvasSlot->SetAlignment(FVector2D(0.0f, 1.0f)); // 아래쪽 정렬
				CanvasSlot->SetPosition(FVector2D(i, 0.0f)); // X, Y 위치 (Y는 고정)
				
				if (RulerTickArray.Num() % 10 == 1) // 간격 10마다 표시
				{
					CanvasSlot->SetSize(FVector2D(2, 40));   // 선의 크기 (2x40)
				}
				else
				{
					CanvasSlot->SetSize(FVector2D(2, 20));   // 선의 크기 (2x20)
				}
				

				//NewRulerTick->SetRenderTransformAngle(90.0f);
				
			}

			

			// 필요시 TextBlock을 추가하여 간격 표시
			//UTextBlock* TickLabel = NewObject<UTextBlock>(this);
			//
			//// TextBlock을 캔버스에 추가
			//CanvasPanel->AddChild(TickLabel);

			//TickLabel->SetText(FText::AsNumber(i));
			//CanvasSlot = Cast<UCanvasPanelSlot>(TickLabel->Slot);
			//if (CanvasSlot)
			//{
			//	CanvasSlot->SetPosition(FVector2D(i, 25)); // 눈금 아래에 숫자 표시
			//}

			
		}
	}
}

void UTimeScaleWidget::SetRulerSpacing(float InRulerSpacing)
{
	// 간격을 설정
	CurrentRulerSpacing = InRulerSpacing;

	// 눈금자 다시 그리기
	DrawRuler();
}

void UTimeScaleWidget::SetZoomScale(float InZoomScale)
{
	if (InZoomScale <= Variables::MinZoomScale || InZoomScale >= Variables::MaxZoomScale)
	{
		return;
	}

	// 확대/축소 비율을 설정
	float OldScale = CurrentZoomScale;
	CurrentZoomScale = InZoomScale;
	float ScaleDiff = CurrentZoomScale - OldScale;
	Variables::ScaleMultiplier += ScaleDiff;
	
	UTimebarPlayer::GetTimebarPlayer()->OnScaleChanged.Broadcast(OldScale, InZoomScale);

	// 눈금자 다시 그리기
	DrawRuler();
}


