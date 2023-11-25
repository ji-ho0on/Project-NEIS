// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterItem.h"
#include "Components/SphereComponent.h"
#include "../../Player/CJH/ShooterCharacter.h"

AShooterItem::AShooterItem()
	: Super()
	, m_ShooterItemState(EShooterItemState::Pickup)
	, m_ShooterItemType(EShooterItemType::MAX)
	, m_ZCurveTime(0.7f)
	, m_ItemInterpX(0.0f)
	, m_ItemInterpY(0.0f)
	, m_InterpInitialYawOffset(0.0f)
	, m_ItemInterpStartLocation(FVector(0.0f))
	, m_ItemDropTime(0.1f)
{
	m_ItemName = TEXT("ShooterItem");
}

void AShooterItem::BeginPlay()
{
	Super::BeginPlay();


	// Setup overlap for CollisionBox
	GetCollisionBox()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);
	GetCollisionBox()->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);

	SetItemProperties(m_ShooterItemState);
}

void AShooterItem::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		AShooterCharacter* Character = Cast<AShooterCharacter>(OtherActor);
		if (IsValid(Character))
			PickedUp(Character);
	}
}

void AShooterItem::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AShooterItem::SetItemProperties(EShooterItemState InState)
{
	switch (InState)
	{
	case EShooterItemState::Pickup:
		GetItemMesh()->SetVisibility(true);
		GetCollisionBox()->SetCollisionProfileName("Item");
		GetCollisionBox()->SetEnableGravity(false);
		GetCollisionBox()->SetSimulatePhysics(false);
		break;
	case EShooterItemState::PickingUp:
		GetItemMesh()->SetVisibility(true);
		GetCollisionBox()->SetCollisionProfileName("NoCollision");
		GetCollisionBox()->SetEnableGravity(false);
		GetCollisionBox()->SetSimulatePhysics(false);

		PlayPickupSound();
		break;
	case EShooterItemState::PickedUp:
		GetItemMesh()->SetVisibility(false);
		GetCollisionBox()->SetCollisionProfileName("NoCollision");
		GetCollisionBox()->SetEnableGravity(false);
		GetCollisionBox()->SetSimulatePhysics(false);

		PlayPickedUpSound();
		break;
	case EShooterItemState::Equipped:
		GetItemMesh()->SetVisibility(true);
		GetCollisionBox()->SetCollisionProfileName("NoCollision");
		GetCollisionBox()->SetEnableGravity(false);
		GetCollisionBox()->SetSimulatePhysics(false);
		break;
	case EShooterItemState::Failling:
		GetItemMesh()->SetVisibility(true);
		GetCollisionBox()->SetCollisionProfileName("NoCollision");
		GetCollisionBox()->SetEnableGravity(true);
		GetCollisionBox()->SetSimulatePhysics(true);
		break;
	}
}

void AShooterItem::PickedUp(AShooterCharacter* InCharacter)
{
	if (!IsValid(InCharacter))
		return;

	SetOwnerCharacter(InCharacter);

	StartItemZCurve();
}

void AShooterItem::StartItemZCurve()
{
	SetShooterItemState(EShooterItemState::PickingUp);

	m_ItemInterpStartLocation = GetActorLocation();

	GetWorldTimerManager().SetTimer(m_ItemInterpTimer, this, &ThisClass::FinishItemZCurve, m_ZCurveTime);

	const float CameraRotationYaw = GetOwnerCharacter()->GetCamera()->GetComponentRotation().Yaw;
	const float ItemRotationYaw = GetActorRotation().Yaw;

	m_InterpInitialYawOffset = ItemRotationYaw - CameraRotationYaw;
}

void AShooterItem::FinishItemZCurve()
{
	if (IsValid(m_OwnerCharacter))
	{
		SetShooterItemState(EShooterItemState::PickedUp);
		
		m_OwnerCharacter->GetPickupItem(this);
	}

	OnFinishItemZCurve();

	// 아이템 크기 원상복귀
	SetActorScale3D(FVector(1.0f));
}

void AShooterItem::OnFinishItemZCurve()
{
}

void AShooterItem::ItemInterp(float DeltaTime)
{
	if (EShooterItemState::PickingUp != m_ShooterItemState)
		return;

	if (!IsValid(m_OwnerCharacter))
		return;

	if (IsValid(m_ItemZCurve))
	{
		const float ElapsedTime = GetWorldTimerManager().GetTimerElapsed(m_ItemInterpTimer);
		// 타이머가 흐른 시간을 이용해 CurveValue를 가져온다.
		const float CuverValue = m_ItemZCurve->GetFloatValue(ElapsedTime);

		// Character에 미리 등록된 위치를 가져온다.
		const FVector CameraInterpLocation = m_OwnerCharacter->GetCameraInterpLocation();

		// 등록된 위치와의 Z값 차이
		FVector ItemLocation = m_ItemInterpStartLocation;
		const FVector ItemToCamera = FVector(0.0f, 0.0f, (CameraInterpLocation - ItemLocation).Z);
		const float DeltaZ = ItemToCamera.Size();

		const FVector CurrentLocation = GetActorLocation();
		const float InterpXValue = FMath::FInterpTo(CurrentLocation.X, CameraInterpLocation.X, DeltaTime, 30.0f);
		const float InterpYValue = FMath::FInterpTo(CurrentLocation.Y, CameraInterpLocation.Y, DeltaTime, 30.0f);

		ItemLocation.X = InterpXValue;
		ItemLocation.Y = InterpYValue;

		// Z값을 CurveValue의 값을 이용해 변경
		ItemLocation.Z += CuverValue * DeltaZ;
		SetActorLocation(ItemLocation, true, nullptr, ETeleportType::TeleportPhysics);

		// 아이템 획득 시에 캐릭터가 돌아가면 부자연스럽게 연출되므로 처음 회전값으로 Rotation 적용
		const FRotator CameraRotation = m_OwnerCharacter->GetCamera()->GetComponentRotation();
		FRotator ItemRotation = FRotator(0.0f, CameraRotation.Yaw + m_InterpInitialYawOffset, 0.0f);
		SetActorRotation(ItemRotation, ETeleportType::TeleportPhysics);

		// 아이템의 크기 조정
		if (IsValid(m_ItemScaleCurve))
		{
			const float ScaleCuverValue = m_ItemScaleCurve->GetFloatValue(ElapsedTime);
			SetActorScale3D(FVector(ScaleCuverValue));
		}
	}
}

void AShooterItem::PlayPickupSound()
{
	if (!IsValid(m_OwnerCharacter))
		return;

	if (IsValid(GetPickupSound()))
		UGameplayStatics::PlaySound2D(this, GetPickupSound());
}

void AShooterItem::PlayPickedUpSound()
{
	if (!IsValid(m_OwnerCharacter))
		return;

	if (IsValid(GetPickedUpSound()))
		UGameplayStatics::PlaySound2D(this, GetPickedUpSound());
}

void AShooterItem::FinishItemDrop()
{
	SetShooterItemState(EShooterItemState::Pickup);
}

void AShooterItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ItemInterp(DeltaTime);
}

void AShooterItem::DropItem()
{
	SetShooterItemState(EShooterItemState::Pickup);

	//GetCollisionBox()->AddVelocityChangeImpulseAtLocation(GetActorUpVector() * 250.0f, GetActorLocation());

	//GetWorldTimerManager().SetTimer(m_ItemDropTimer, this, &ThisClass::FinishItemDrop, m_ItemDropTime);
}

void AShooterItem::SetOwnerCharacter(AShooterCharacter* InCharacter)
{
	m_OwnerCharacter = InCharacter;
}

void AShooterItem::SetShooterItemState(EShooterItemState InState)
{
	m_ShooterItemState = InState;

	SetItemProperties(InState);
}

void AShooterItem::SetShooterItemType(EShooterItemType InType)
{
	m_ShooterItemType = InType;
}
