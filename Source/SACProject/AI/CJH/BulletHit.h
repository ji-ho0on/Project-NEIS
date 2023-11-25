// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "UObject/Interface.h"
#include "BulletHit.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBulletHit : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SACPROJECT_API IBulletHit
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BulletHit(const FHitResult& HitResult, float BulletDamage, AController* HitInstigator, AActor* DamageCauser);
};
