// Fill out your copyright notice in the Description page of Project Settings.


#include "DragDrop/TimebarDragDropDependency.h"

namespace Variables
{
	float SnapInterval = 10.f;  // 최소 10픽셀 단위로 스냅
	float ConvertedValue = 50.0f; // DeltaSecond <-> Viewport
	float ScaleMultiplier = 1.0f;
	float MinClipLength = 30.0f; // 최소 클립 길이 설정
	float ResizeThreshold = 10.0f;
	float InitialRulerSpacing = 10.0f; // 초기 스케일 간격
	float InitialZoomScale = 1.0f; // 초기 줌 스케일
	float MaxZoomScale = 2.0f;
	float MinZoomScale = 0.4f;
	float CursorInterval = 300.0f; // 커서가 인터벌 간격 도달시 스크롤 자동 조정
}
