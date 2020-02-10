// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 *
 */
UCLASS()
class BELLZ_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEnemyAIController();

	virtual void OnPossess(class APawn* InPawn) override;
	virtual void BeginInactiveState() override;
	void Respawn();

	UPROPERTY(transient)
	UBlackboardComponent*	BlackboardComp;

	UPROPERTY(transient)
	class UBehaviorTreeComponent*	BehaviorComp;

	UFUNCTION(BlueprintCallable, Category = Behavior)
	void SetEnemy(class APawn* InPawn);

	UFUNCTION(BlueprintCallable, Category = Behavior)
	class AGladiator* GetEnemy() const;

	UFUNCTION(BlueprintCallable, Category = Behavior)
	void UpdateControlRotation(float DeltaTime, bool bUpdatePawn);

	UFUNCTION(BlueprintCallable, Category = Behavior)
	bool PawnCanBeSeen(class APawn* taget);

	UFUNCTION(BlueprintCallable, CAtegory = Behavior)
	void OnSearchEnemy();

protected:
	int32 EnemyKeyID;
	int32 EnemyPositionKeyID;
	FTimerHandle SpawnTimer;
};
