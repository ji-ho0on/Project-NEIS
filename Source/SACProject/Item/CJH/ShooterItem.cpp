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

	// ������ ũ�� ���󺹱�
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
		// Ÿ�̸Ӱ� �帥 �ð��� �̿��� CurveValue�� �����´�.
		const float CuverValue = m_ItemZCurve->GetFloatValue(ElapsedTime);

		// Character�� �̸� ��ϵ� ��ġ�� �����´�.
		const FVector CameraInterpLocation = m_OwnerCharacter->GetCameraInterpLocation();

		// ��ϵ� ��ġ���� Z�� ����
		FVector ItemLocation = m_ItemInterpStartLocation;
		const FVector ItemToCamera = FVector(0.0f, 0.0f, (CameraInterpLocation - ItemLocation).Z);
		const float DeltaZ = ItemToCamera.Size();

		const FVector CurrentLocation = GetActorLocation();
		const float InterpXValue = FMath::FInterpTo(CurrentLocation.X, CameraInterpLocation.X, DeltaTime, 30.0f);
		const float InterpYValue = FMath::FInterpTo(CurrentLocation.Y, CameraInterpLocation.Y, DeltaTime, 30.0f);

		ItemLocation.X = InterpXValue;
		ItemLocation.Y = InterpYValue;

		// Z���� CurveValue�� ���� �̿��� ����
		ItemLocation.Z += CuverValue * DeltaZ;
		SetActorLocation(ItemLocation, true, nullptr, ETeleportType::TeleportPhysics);

		// ������ ȹ�� �ÿ� ĳ���Ͱ� ���ư��� ���ڿ������� ����ǹǷ� ó�� ȸ�������� Rotation ����
		const FRotator CameraRotation = m_OwnerCharacter->GetCamera()->GetComponentRotation();
		FRotator ItemRotation = FRotator(0.0f, CameraRotation.Yaw + m_InterpInitialYawOffset, 0.0f);
		SetActorRotation(ItemRotation, ETeleportType::TeleportPhysics);

		// �������� ũ�� ����
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
