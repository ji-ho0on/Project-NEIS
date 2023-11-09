// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterMonsterController.h"

AShooterMonsterController::AShooterMonsterController()
	: Super()
{
}

void AShooterMonsterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsValid(m_AITree) && IsValid(m_AIBlackboard))
	{
		UBlackboardComponent* BlackboardRef = Blackboard;

		if (UseBlackboard(m_AIBlackboard, BlackboardRef))
		{
			RunBehaviorTree(m_AITree);

			BlackboardRef->SetValueAsFloat(TEXT("DistanceToTarget"), 99999.0f);
		}
	}
}
