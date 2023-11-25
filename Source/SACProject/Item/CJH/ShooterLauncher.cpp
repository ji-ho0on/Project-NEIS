// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterLauncher.h"
#include "../../Player/CJH/ShooterCharacter.h"
#include "../../Projectile/CJH/ShooterProjectileBase.h"

AShooterLauncher::AShooterLauncher()
	: Super()
{
}

void AShooterLauncher::SendBullet()
{
	const USkeletalMeshSocket* Muzzle = GetWeaponMesh()->GetSocketByName(TEXT("Muzzle"));
	if (IsValid(Muzzle))
	{
		const FTransform MuzzleSocketTrans = Muzzle->GetSocketTransform(GetWeaponMesh());

		if (IsValid(m_MuzzleFlashParticle))
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_MuzzleFlashParticle, MuzzleSocketTrans);

		FVector BeamEndLocation;
		FHitResult CrosshairHitResult;
		bool bCrosshairHit = GetOwnerCharacter()->TraceUnderCrosshairs(CrosshairHitResult, BeamEndLocation);
		if (bCrosshairHit)
		{
			// ¼¦ ¹æÇâ°è»ê
			const FRotator ShotDir = (BeamEndLocation - MuzzleSocketTrans.GetLocation()).Rotation();

			// À¯Åº ½ºÆù
			if (IsValid(m_GrenadeProjectileClass))
				GetWorld()->SpawnActor<AShooterProjectileBase>(m_GrenadeProjectileClass, MuzzleSocketTrans.GetLocation(), ShotDir);
		}
	}
}
