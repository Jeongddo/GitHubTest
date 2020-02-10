// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Gladiator.h"
#include "Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "GameFramework/Controller.h"
#include "Engine/Engine.h"
#include "GameFramework/GameState.h"
#include "TimerManager.h"

AEnemyAIController::AEnemyAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

	bWantsPlayerState = true;
}
//--------------------------------------------------------------------------------------------------------------------------//
void AEnemyAIController::OnPossess(class APawn* InPawn)
{
	if(nullptr == InPawn)
		return;

	Super::OnPossess(InPawn);

	AEnemy* _tempEnemy = Cast<AEnemy>(InPawn);

	if (nullptr != _tempEnemy && _tempEnemy->EnemyBehaviorTree)
	{
		BlackboardComp->InitializeBlackboard(*_tempEnemy->EnemyBehaviorTree->BlackboardAsset);
	
		EnemyKeyID = BlackboardComp->GetKeyID("Enemy");
		EnemyPositionKeyID = BlackboardComp->GetKeyID("EnemyPosition");
		BehaviorComp->StartTree(*_tempEnemy->EnemyBehaviorTree);
		UE_LOG(LogClass, Log, TEXT("=========>>>> Got the enemy and the BT have been started"));
	}
}
//--------------------------------------------------------------------------------------------------------------------------//
void AEnemyAIController::BeginInactiveState()
{
	Super::BeginInactiveState();
	const float MinRespawnDelay = 1.f;
	
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemyAIController::Respawn, MinRespawnDelay);
}
//--------------------------------------------------------------------------------------------------------------------------//
void AEnemyAIController::Respawn()
{

}
//--------------------------------------------------------------------------------------------------------------------------//
void AEnemyAIController::SetEnemy(class APawn* InPawn)
{
	if (nullptr != BlackboardComp)
	{
		const FString keyid = FString::FromInt(EnemyPositionKeyID);
		const FName keyName = FName(*keyid);
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(EnemyKeyID, InPawn);
		BlackboardComp->SetValueAsVector(keyName, InPawn->GetActorLocation());
		SetFocus(InPawn);
	}
}
//--------------------------------------------------------------------------------------------------------------------------//
class AGladiator* AEnemyAIController::GetEnemy() const
{
	if (nullptr != BlackboardComp)
	{
		return Cast<AGladiator>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));
	}

	return nullptr;
}
//--------------------------------------------------------------------------------------------------------------------------//
void AEnemyAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	FVector TheCenter = GetFocalPoint();
	if (!TheCenter.IsZero() && GetPawn())
	{
		FVector Direction = TheCenter - GetPawn()->GetActorLocation();
		FRotator TheNewRotation = Direction.Rotation();
		
		TheNewRotation.Yaw = FRotator::ClampAxis(TheNewRotation.Yaw); // 0~ 360
		
		SetControlRotation(TheNewRotation);
		
		APawn* const _tempPawn = GetPawn();
		if (nullptr != _tempPawn && bUpdatePawn)
		{
			_tempPawn->FaceRotation(TheNewRotation, DeltaTime);
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------//
bool AEnemyAIController::PawnCanBeSeen(class APawn* taget)
{
	if(nullptr == taget || GetPawn() == nullptr)
		return false;
	FVector difference = taget->GetActorLocation() - GetPawn()->GetActorLocation();
	float angle = FVector::DotProduct(difference, GetPawn()->GetActorRotation().Vector());
	
	return  LineOfSightTo(taget, GetPawn()->GetActorLocation()) && (angle > 0);
}
//--------------------------------------------------------------------------------------------------------------------------//
void AEnemyAIController::OnSearchEnemy()
{
	APawn* _tempPawn = GetPawn();
	if (nullptr == _tempPawn)
	{
		return;
	}
	FString message = TEXT("=== OnSearchEnemy ==== ");
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, message);
	const FVector _tempLocation = _tempPawn->GetActorLocation();
	float BestDistSq = MAX_FLT;
	AGladiator* PlayerPawn = nullptr;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* pawn = Cast<APawn>(*It);
		if (PawnCanBeSeen(pawn))
		{
			AGladiator* TestPawn = Cast<AGladiator>(pawn);
			AEnemy* const _testEnemy = Cast<AEnemy>(TestPawn);

			if (nullptr != _testEnemy)
			{

			}
			else
			{
				if (nullptr != TestPawn && TestPawn->GetIsStillAlive())
				{
					UE_LOG(LogClass, Log, TEXT("==============>> ENEMY SEEN %s"), *GetNameSafe(pawn));
					const float _distanceSq = (TestPawn->GetActorLocation() - _tempLocation).SizeSquared();
					if (_distanceSq < BestDistSq)
					{
						BestDistSq = _distanceSq;
						PlayerPawn = TestPawn;
					}
				}
			}
		}
	}

	if (nullptr != PlayerPawn)
	{
		SetEnemy(PlayerPawn);
		UE_LOG(LogClass, Log, TEXT("==============>>Set Target"));
	}
}
