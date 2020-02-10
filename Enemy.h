// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class BELLZ_API AEnemy : public ACharacter
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Triggers, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* bodySphereTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Triggers, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* leftHandTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Triggers, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* rightHandTrigger;
public:
	// Sets default values for this character's properties
	AEnemy();

	virtual void PostInitializeComponents() override;	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior")
	float TotalHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior")
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior")
	bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior")
	bool IsAttacking;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy AI")
	class UPawnSensingComponent*	PawnSensor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy AI")
	class UBehaviorTree* EnemyBehaviorTree;
public:
	UFUNCTION(BlueprintCallable, Category = "Enemy AI")
	void OnPerformAttack();
	
	UFUNCTION(BlueprintCallable, Category = "Enemy AI")
	void OnPreAttack();

	UFUNCTION(BlueprintCallable, Category = "Enemy AI")
	void OnPostAttack();

	UFUNCTION()
	void OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume);
	
	UFUNCTION()
	void OnSeePawn(APawn* OtherPawn);

	UFUNCTION()
	void OnHandTriggerOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherCOmp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FORCEINLINE class USphereComponent* GetBodySphereTrigger() const { return bodySphereTrigger; }
	
};
