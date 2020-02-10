// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gladiator.generated.h"

UCLASS()
class BELLZ_API AGladiator : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGladiator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Effects, meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* EffectSprite;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Attributes")
	float jumppingVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Attributes")
	bool IsStillAlive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Attributes")
	bool IsAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Attributes")
	int32 WeaponIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Attributes")
	bool IsControlable;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game DataTables")

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	bool GetIsStillAlive() const { return IsStillAlive; }
	
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	void OnSetPlayerController(bool status);
	
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	void OnChangeHealthByAmount(float usedAmount);

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float OnGetHealthAmount() const {return TotalHealth;}
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	void OnPosAttack();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
protected:
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void MoveForward(float Value);
	
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void MoveRight(float Value);
	
	//UFUNCTION(BlueprintCallable, Category = "Player Actions")
	virtual void Jump() override;
	//UFUNCTION(BlueprintCallable, Category = "Player Actions")
	virtual void StopJumping() override;
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void OnAttack();
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
	void OnChangeWeapon();

	UFUNCTION(BlueprintCallable, Category = "Player Pickup")
	void OnCollectPickup();
	
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Attributes")
	float TotalHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Attributes")
	float AttackRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Pickup")
	int32 CollectedCoinsValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Pickup")
	int32 CollectedCoins;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
