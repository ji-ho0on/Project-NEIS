// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterExplosive.h"
#include "../../Collider/CJH/ShooterColliderBase.h"

AShooterExplosive::AShooterExplosive()
	: Super()
{
}

void AShooterExplosive::BulletHit_Implementation(const FHitResult& HitResult, float BulletDamage, AController* HitInstigator, AActor* DamageCauser)
{
	if (IsValid(m_ExplosionSound))
		UGameplayStatics::PlaySoundAtLocation(this, m_ExplosionSound, GetActorLocation());

	if (IsValid(m_ExplosionParticle))
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_ExplosionParticle, HitResult.Location);

	if (IsValid(m_HitBoxClass))
	{
		FActorSpawnParameters Params;
		Params.Owner = this;

		GetWorld()->SpawnActor<AShooterColliderBase>(m_HitBoxClass, GetActorLocation(), GetActorRotation(), Params);
	}

	Destroy();
}
