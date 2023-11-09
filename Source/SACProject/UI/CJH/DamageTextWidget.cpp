// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageTextWidget.h"

UDamageTextWidget::UDamageTextWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, m_DestroyTime(1.5f)
	, m_DestroyTimeAcc(0.0f)
{
}

void UDamageTextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_DamageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HitDamage")));

	if (IsValid(DamageTextAnim))
		PlayAnimation(DamageTextAnim);
}

void UDamageTextWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (m_DestroyTimeAcc >= m_DestroyTime)
		RemoveFromParent();

	m_DestroyTimeAcc += InDeltaTime;

	float OpacityPercent = (m_DestroyTime - m_DestroyTimeAcc) / m_DestroyTime;
	SetRenderOpacity(OpacityPercent);

	UpdateLocation();
}

void UDamageTextWidget::UpdateLocation()
{
	FVector2D ScreenPosition;
	UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), m_DamageLocation, ScreenPosition);
	SetPositionInViewport(ScreenPosition);
}

void UDamageTextWidget::SetDamageText(int32 Damage, const FVector& HitLocation)
{
	if (IsValid(m_DamageText))
		m_DamageText->SetText(FText::AsNumber(Damage));

	m_DamageLocation = HitLocation;
}
