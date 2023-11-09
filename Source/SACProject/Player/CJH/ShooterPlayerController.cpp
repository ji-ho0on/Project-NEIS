// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "ShooterCharacter.h"
#include "../../UI/CJH/ShooterOverlayWidget.h"

AShooterPlayerController::AShooterPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// EnhancedInput Subsystem Init
	UEnhancedInputLocalPlayerSubsystem* eiSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (eiSubsystem)
	{
		eiSubsystem->ClearAllMappings();
		eiSubsystem->AddMappingContext(m_ShooterIMC, 0);
	}

	PlayerCameraManager->ViewPitchMin = m_ViewPitchLimit.X;
	PlayerCameraManager->ViewPitchMax = m_ViewPitchLimit.Y;

	if (m_OverlayWidgetClass)
	{
		m_OverlayWidget = CreateWidget<UShooterOverlayWidget>(this, m_OverlayWidgetClass);
		if (IsValid(m_OverlayWidget))
		{
			m_OverlayWidget->AddToViewport();
			m_OverlayWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* eInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (IsValid(eInputComponent))
	{
		// bind action
		if (IsValid(m_MoveIA))
			eInputComponent->BindAction(m_MoveIA, ETriggerEvent::Triggered, this, &ThisClass::MoveAxis);

		if (IsValid(m_MouseIA))
			eInputComponent->BindAction(m_MouseIA, ETriggerEvent::Triggered, this, &ThisClass::MouseAxis);

		if (IsValid(m_LeftClickIA))
		{
			eInputComponent->BindAction(m_LeftClickIA, ETriggerEvent::Started, this, &ThisClass::StartLeftClick);
			eInputComponent->BindAction(m_LeftClickIA, ETriggerEvent::Completed, this, &ThisClass::StopLeftClick);
		}

		if (IsValid(m_RightClickIA))
		{
			eInputComponent->BindAction(m_RightClickIA, ETriggerEvent::Started, this, &ThisClass::StartRightClick);
			eInputComponent->BindAction(m_RightClickIA, ETriggerEvent::Completed, this, &ThisClass::StopRightClick);
		}

		if (IsValid(m_SpaceBarIA))
		{
			eInputComponent->BindAction(m_SpaceBarIA, ETriggerEvent::Started, this, &ThisClass::StartSpaceBar);
			eInputComponent->BindAction(m_SpaceBarIA, ETriggerEvent::Completed, this, &ThisClass::StopSpaceBar);
		}

		if (IsValid(m_CtrlKeyIA))
		{
			eInputComponent->BindAction(m_CtrlKeyIA, ETriggerEvent::Started, this, &ThisClass::StartCtrlKey);
			eInputComponent->BindAction(m_CtrlKeyIA, ETriggerEvent::Completed, this, &ThisClass::StopCtrlKey);
		}

		if (IsValid(m_ShiftKeyIA))
		{
			eInputComponent->BindAction(m_ShiftKeyIA, ETriggerEvent::Started, this, &ThisClass::StartShiftKey);
			eInputComponent->BindAction(m_ShiftKeyIA, ETriggerEvent::Completed, this, &ThisClass::StopShiftKey);
		}

		if (IsValid(m_RkeyIA))
		{
			eInputComponent->BindAction(m_RkeyIA, ETriggerEvent::Started, this, &ThisClass::StartRKey);
			eInputComponent->BindAction(m_RkeyIA, ETriggerEvent::Completed, this, &ThisClass::StopRKey);
		}

		if (IsValid(m_QkeyIA))
		{
			eInputComponent->BindAction(m_QkeyIA, ETriggerEvent::Started, this, &ThisClass::StartQKey);
			eInputComponent->BindAction(m_QkeyIA, ETriggerEvent::Completed, this, &ThisClass::StopQKey);
		}

		if (IsValid(m_EkeyIA))
		{
			eInputComponent->BindAction(m_EkeyIA, ETriggerEvent::Started, this, &ThisClass::StartEKey);
			eInputComponent->BindAction(m_EkeyIA, ETriggerEvent::Completed, this, &ThisClass::StopEKey);
		}

		if (IsValid(m_1keyIA))
		{
			eInputComponent->BindAction(m_1keyIA, ETriggerEvent::Started, this, &ThisClass::Start1Key);
			eInputComponent->BindAction(m_1keyIA, ETriggerEvent::Completed, this, &ThisClass::Stop1Key);
		}

		if (IsValid(m_2keyIA))
		{
			eInputComponent->BindAction(m_2keyIA, ETriggerEvent::Started, this, &ThisClass::Start2Key);
			eInputComponent->BindAction(m_2keyIA, ETriggerEvent::Completed, this, &ThisClass::Stop2Key);
		}

		if (IsValid(m_3keyIA))
		{
			eInputComponent->BindAction(m_3keyIA, ETriggerEvent::Started, this, &ThisClass::Start3Key);
			eInputComponent->BindAction(m_3keyIA, ETriggerEvent::Completed, this, &ThisClass::Stop3Key);
		}

		if (IsValid(m_4keyIA))
		{
			eInputComponent->BindAction(m_4keyIA, ETriggerEvent::Started, this, &ThisClass::Start4Key);
			eInputComponent->BindAction(m_4keyIA, ETriggerEvent::Completed, this, &ThisClass::Stop4Key);
		}
	}
}

void AShooterPlayerController::MoveAxis(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->MoveAxis(Value);
	}
}

void AShooterPlayerController::MouseAxis(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->MouseAxis(Value);
	}
}

void AShooterPlayerController::StartLeftClick(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->StartLeftClick(Value);
	}
}

void AShooterPlayerController::StopLeftClick(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->StopLeftClick(Value);
	}
}

void AShooterPlayerController::StartRightClick(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->StartRightClick(Value);
	}
}

void AShooterPlayerController::StopRightClick(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->StopRightClick(Value);
	}
}

void AShooterPlayerController::StartSpaceBar(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->StartSpaceBar(Value);
	}
}

void AShooterPlayerController::StopSpaceBar(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->StartSpaceBar(Value);
	}
}

void AShooterPlayerController::StartCtrlKey(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->StartCtrlKey(Value);
	}
}

void AShooterPlayerController::StopCtrlKey(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->StopCtrlKey(Value);
	}
}

void AShooterPlayerController::StartShiftKey(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->StartShiftKey(Value);
	}
}

void AShooterPlayerController::StopShiftKey(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->StopShiftKey(Value);
	}
}

void AShooterPlayerController::StartRKey(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->StartRKey(Value);
	}
}

void AShooterPlayerController::StopRKey(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->StopRKey(Value);
	}
}

void AShooterPlayerController::StartQKey(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->StartQKey(Value);
	}
}

void AShooterPlayerController::StopQKey(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->StopQKey(Value);
	}
}

void AShooterPlayerController::StartEKey(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->StartEKey(Value);
	}
}

void AShooterPlayerController::StopEKey(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->StopEKey(Value);
	}
}

void AShooterPlayerController::Start1Key(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->Start1Key(Value);
	}
}

void AShooterPlayerController::Stop1Key(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->Stop1Key(Value);
	}
}

void AShooterPlayerController::Start2Key(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->Start2Key(Value);
	}
}

void AShooterPlayerController::Stop2Key(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->Stop2Key(Value);
	}
}

void AShooterPlayerController::Start3Key(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->Start3Key(Value);
	}
}

void AShooterPlayerController::Stop3Key(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->Stop3Key(Value);
	}
}

void AShooterPlayerController::Start4Key(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->Start4Key(Value);
	}
}

void AShooterPlayerController::Stop4Key(const FInputActionValue& Value)
{
	if (AShooterCharacter* ShooterCharacter = GetShooterCharacter())
	{
		ShooterCharacter->Stop4Key(Value);
	}
}

AShooterCharacter* AShooterPlayerController::GetShooterCharacter() const
{
	return CastChecked<AShooterCharacter>(K2_GetPawn());
}
