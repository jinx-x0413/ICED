// Fill out your copyright notice in the Description page of Project Settings.


#include "Table/TableContainer.h"
#include "TableDependency.h"


void UTableContainer::InitializeTable(UTableManager* InManager)
{
	if (IsValid(InManager))
	{
		TargetManager = InManager;
		if (!TargetManager->OnTableCreated.IsAlreadyBound(this, &UTableContainer::SetHeader))
		{
			TargetManager->OnTableCreated.AddDynamic(this, &UTableContainer::SetHeader);
		}
	}


}

FString UTableContainer::GetRowValue(int RowIndex, int ColumnIndex)
{
	return FString();
}

void UTableContainer::SetRowValue(int RowIndex, int ColumnIndex, const FString& NewValue)
{
}

void UTableContainer::AddRow(UTableRow* InRowWidget)
{
	if (IsValid(InRowWidget))
	{
		Rows.Add(InRowWidget);
		OnTableRowAdded.Broadcast(InRowWidget);
	}
}

void UTableContainer::RemoveRow(UTableRow* InRowWidget)
{
	if (IsValid(InRowWidget))
	{
		Rows.Remove(InRowWidget);
		OnTableRowRemoved.Broadcast(InRowWidget);
	}
}



void UTableContainer::SetHeader()
{
	if (Rows.Num() == 0)
	{
		return;
	}

	ExecSetHeader(Rows[0]->TargetRowData);
}


bool IsOverlapping(const FVector2D& DropPosition, const FVector2D& WidgetPosition, const FVector2D& WidgetSize)
{

	return (DropPosition.X < WidgetPosition.X + WidgetSize.X &&
		DropPosition.X + WidgetSize.X > WidgetPosition.X &&
		DropPosition.Y < WidgetPosition.Y + WidgetSize.Y &&
		DropPosition.Y + WidgetSize.Y > WidgetPosition.Y);
};


bool UTableContainer::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UMyDragDropOperation* DragOp = Cast<UMyDragDropOperation>(InOperation))
	{
		if (UDraggableWidget* DraggedWidget = Cast<UDraggableWidget>(DragOp->SourceWidget))
		{
			FVector2D DropPosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
			FVector2D SnappedPosition = GetSnappedPosition(DropPosition);  // 예: 스냅 위치 계산

			FVector2D NewPosition = FVector2D(SnappedPosition.X - DragOp->DragOffset.X, SnappedPosition.Y - DragOp->DragOffset.Y);
			FVector2D LocalNewPosition = InGeometry.AbsoluteToLocal(NewPosition);
			if (!IsValid(TargetScrollBox))
			{
				return false;
			}

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("NewLocalPosition : %f, %f"),
				LocalNewPosition.X, LocalNewPosition.Y));

			// 스크롤박스의 위치를 구함 (스크롤박스가 상대 좌표를 사용할 수 있게 위치 계산)
			//FGeometry ScrollGeometry = TargetScrollBox->GetCachedGeometry();
			//FVector2D ScrollBoxPosition = ScrollGeometry.GetAbsolutePosition();
			//float ScrollOffset = ScrollGeometry.GetAccumulatedLayoutTransform().GetTranslation().Y;


			// 스크롤박스 내의 모든 자식 위젯을 가져옴
			//TArray<UWidget*> AllChildren = TargetScrollBox->GetAllChildren();
			//for (UWidget* ChildWidget : AllChildren)
			//{
			//	if (UWidget* Widget = Cast<UWidget>(ChildWidget))
			//	{
			//		// 자식 위젯의 Geometry를 가져옴
			//		FGeometry WidgetGeometry = Widget->GetCachedGeometry();

			//		// 자식 위젯의 절대 위치 계산 (스크롤박스의 위치와 스크롤 상태를 반영)
			//		FVector2D WidgetPosition = ScrollBoxPosition + WidgetGeometry.GetAbsolutePosition() - FVector2D(0, ScrollOffset);
			//		FVector2D WidgetSize = WidgetGeometry.GetLocalSize();

			//		// 겹치는지 체크
			//		if (IsOverlapping(NewPosition, WidgetPosition, WidgetSize))
			//		{
			//			// 겹치는 위젯이 있다면, 해당 위젯을 처리하는 로직
			//			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("Overlapping Widget: %s"), *Widget->GetName()));

			//			// 겹치는 위젯을 반환하거나 적절히 처리
			//			return true;
			//		}

			//		// 디버깅: 위젯의 절대 좌표와 크기 출력
			//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Children Widget: %s, Position: (%f, %f), Size: (%f, %f)"),
			//			*Widget->GetName(), WidgetPosition.X, WidgetPosition.Y, WidgetSize.X, WidgetSize.Y));
			//	}
			//}
		}
	}

	return false;
}
