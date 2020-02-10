// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

UCLASS()
class BELLZ_API APickupBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

 	UFUNCTION(BlueprintPure, Category = "Pickups")
	bool IsActive();
 	UFUNCTION(BlueprintCallable, Category = "Pickups")
	void setActive(bool NewPickupState);

	UFUNCTION(BlueprintNativeEvent)
	void OnGetCollected();
	virtual void OnGetCollected_Implementation();

	FORCEINLINE class UStaticMeshComponent* getMesh() const {return ThePickupStaticMesh;}
 private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickups", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ThePickupStaticMesh;
private:
	bool bIsActive;
};
