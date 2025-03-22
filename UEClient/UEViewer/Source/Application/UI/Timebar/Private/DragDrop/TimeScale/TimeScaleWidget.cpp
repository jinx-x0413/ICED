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

	// GetCachedGeometry()�� ��ȿ���� Ȯ���ϰ�, �ѹ��� DrawRuler�� ȣ��
	
	if (GetCachedGeometry().GetLocalSize().X > 0)
	{
		DrawRuler();  // ������ �׸���
		bIsRulerDrawn = true;  // �� ���� �׷������� ����
	}
}





// drag
FReply UTimeScaleWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		// click
		FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
		FVector2D LocalMousePosition = InGeometry.AbsoluteToLocal(MousePosition); // Local space�� ��ȯ

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
		// ���콺 ��ġ ������Ʈ
		FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
		FVector2D LocalMousePosition = InGeometry.AbsoluteToLocal(MousePosition);

		// Time ������Ʈ
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
	// �ִ� ������ �ʱ�ȭ
	SetMaxScale();

	// ���� �ʱ�ȭ
	if (!RulerTickArray.IsEmpty())
	{
		for (auto& Tick : RulerTickArray)
		{
			Tick->RemoveFromParent();
		}
		RulerTickArray.Empty();
	}

	// ���� ĵ������ �����ɴϴ�.
	UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(GetRootWidget());
	
	if (CanvasPanel)
	{
		// �����ڿ� �׸� �� ������ ũ��
		float SpacingAdjusted = CurrentRulerSpacing * CurrentZoomScale;

		// ������ �׸��� (���ݿ� �´� �� �׸���)
		for (float i = 0; i < MaxScale; i += SpacingAdjusted)
		{
			// �� ������ ��ġ�� �´� Border �Ǵ� Line�� �׸��ϴ�.
			UBorder* NewRulerTick = NewObject<UBorder>(this);

			// �⺻ Border ������
			NewRulerTick->SetBrushColor(FLinearColor::White);
			NewRulerTick->SetPadding(FMargin(0));

			// ĵ������ �߰�
			CanvasPanel->AddChild(NewRulerTick);
			RulerTickArray.Add(NewRulerTick);

			// Canvas�� ��ġ
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(NewRulerTick->Slot);
			if (CanvasSlot)
			{
				CanvasSlot->SetAnchors(FAnchors(0.0f, 1.0f, 0.0f, 1.0f));  // X=0.5f (���), Y=1.0f (�ϴ�)
				CanvasSlot->SetAlignment(FVector2D(0.0f, 1.0f)); // �Ʒ��� ����
				CanvasSlot->SetPosition(FVector2D(i, 0.0f)); // X, Y ��ġ (Y�� ����)
				
				if (RulerTickArray.Num() % 10 == 1) // ���� 10���� ǥ��
				{
					CanvasSlot->SetSize(FVector2D(2, 40));   // ���� ũ�� (2x40)
				}
				else
				{
					CanvasSlot->SetSize(FVector2D(2, 20));   // ���� ũ�� (2x20)
				}
				

				//NewRulerTick->SetRenderTransformAngle(90.0f);
				
			}

			

			// �ʿ�� TextBlock�� �߰��Ͽ� ���� ǥ��
			//UTextBlock* TickLabel = NewObject<UTextBlock>(this);
			//
			//// TextBlock�� ĵ������ �߰�
			//CanvasPanel->AddChild(TickLabel);

			//TickLabel->SetText(FText::AsNumber(i));
			//CanvasSlot = Cast<UCanvasPanelSlot>(TickLabel->Slot);
			//if (CanvasSlot)
			//{
			//	CanvasSlot->SetPosition(FVector2D(i, 25)); // ���� �Ʒ��� ���� ǥ��
			//}

			
		}
	}
}

void UTimeScaleWidget::SetRulerSpacing(float InRulerSpacing)
{
	// ������ ����
	CurrentRulerSpacing = InRulerSpacing;

	// ������ �ٽ� �׸���
	DrawRuler();
}

void UTimeScaleWidget::SetZoomScale(float InZoomScale)
{
	if (InZoomScale <= Variables::MinZoomScale || InZoomScale >= Variables::MaxZoomScale)
	{
		return;
	}

	// Ȯ��/��� ������ ����
	float OldScale = CurrentZoomScale;
	CurrentZoomScale = InZoomScale;
	float ScaleDiff = CurrentZoomScale - OldScale;
	Variables::ScaleMultiplier += ScaleDiff;
	
	UTimebarPlayer::GetTimebarPlayer()->OnScaleChanged.Broadcast(OldScale, InZoomScale);

	// ������ �ٽ� �׸���
	DrawRuler();
}


