// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterShotgun.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "../../Player/CJH/ShooterCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../AI/CJH/BulletHit.h"
#include "../../AI/CJH/ShooterMonster.h"

AShooterShotgun::AShooterShotgun()
	: Super()
{
}

void AShooterShotgun::SendBullet()
{
	const USkeletalMeshSocket* Muzzle = GetWeaponMesh()->GetSocketByName(TEXT("Muzzle"));
	if (IsValid(Muzzle))
	{
		const FTransform MuzzleSocketTrans = Muzzle->GetSocketTransform(GetWeaponMesh());

		if (IsValid(m_MuzzleFlashParticle))
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_MuzzleFlashParticle, MuzzleSocketTrans);

		TArray<FVector> ImpactPoints;

		FVector BeamEndLocation;
		FHitResult CrosshairHitResult;
		GetOwnerCharacter()->TraceUnderCrosshairs(CrosshairHitResult, BeamEndLocation);
		for (int32 i = 0; i < 9; ++i)
		{
			// �ϴ� �� ����Ʈ�� ����
			FVector ShotPoint = BeamEndLocation;

			// �� ������
			FVector ShotDir = BeamEndLocation - MuzzleSocketTrans.GetLocation();
			ShotDir.Normalize();
			FRotator ShotRotation = UKismetMathLibrary::MakeRotFromX(UKismetMathLibrary::RandomUnitVectorInConeInDegrees(ShotDir, 5.0f));

			// �� ���� ������ Muzzle
			const FVector FireStart = MuzzleSocketTrans.GetLocation();
			
			// �� �� ������ ������������ �������� ���� �������� �����Ÿ���ŭ
			const FVector FireEnd = FireStart + ShotRotation.Vector() * m_WeaponFireRange;

			// �� ����Ʈ ����
			ShotPoint = FireEnd;

			FCollisionQueryParams Param;
			Param.AddIgnoredActor(this);
			Param.AddIgnoredActor(GetOwnerCharacter());

			FHitResult FireHitResult;
			GetWorld()->LineTraceSingleByChannel(FireHitResult, FireStart, FireEnd, ECollisionChannel::ECC_Visibility, Param);
			if (FireHitResult.bBlockingHit)
			{
				// ���� ������ ������ �� ����Ʈ ����
				ShotPoint = FireHitResult.Location;

				if (IsValid(FireHitResult.GetActor()))
				{
					// ���� bullethit �������̽��� ��ӹ��� actor�� ����ٸ�
					IBulletHit* BulletHitInterface = Cast<IBulletHit>(FireHitResult.GetActor());
					if (BulletHitInterface)
					{
						int32 RandomDamage = FMath::RandRange(-5, 5);
						BulletHitInterface->Execute_BulletHit(FireHitResult.GetActor(), FireHitResult, GetWeaponDamage() + RandomDamage, GetOwnerCharacter()->GetController(), this);
					}
					else
					{
						if (IsValid(m_ImpactParticle))
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_ImpactParticle, ShotPoint);
					}
				}
			}

			ImpactPoints.Add(ShotPoint);
		}

		if (IsValid(m_BeamParticleSystem))
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_BeamParticleSystem, MuzzleSocketTrans.GetLocation());

			NiagaraComp->SetNiagaraVariableBool(TEXT("User.Trigger"), true);
			NiagaraComp->SetNiagaraVariableVec3(TEXT("User.MuzzlePosition"), MuzzleSocketTrans.GetLocation());

			UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(NiagaraComp, TEXT("User.ImpactPositions"), ImpactPoints);
		}
	}
}