// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterScoreWidget.h"

UShooterScoreWidget::UShooterScoreWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UShooterScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_CurrentScoreText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentScoreText")));
	m_MaxScoreText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxScoreText")));
}

void UShooterScoreWidget::SetCurrentScoreText(int32 Score)
{
	if (IsValid(m_CurrentScoreText))
		m_CurrentScoreText->SetText(FText::AsNumber(Score));
}

void UShooterScoreWidget::PlayCurrentScoreAnim()
{
	if (IsValid(CurrentScoreTextAnim))
		PlayAnimation(CurrentScoreTextAnim);
}

void UShooterScoreWidget::SetMaxScoreText(int32 Score)
{
	if (IsValid(m_MaxScoreText))
		m_MaxScoreText->SetText(FText::AsNumber(Score));
}
