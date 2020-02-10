// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBase.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APickupBase::APickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bIsActive = true;
	ThePickupStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	RootComponent = ThePickupStaticMesh;
}
//--------------------------------------------------------------------------------------------------------------------------//
// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();
	
}
//--------------------------------------------------------------------------------------------------------------------------//
// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//--------------------------------------------------------------------------------------------------------------------------//
bool APickupBase::IsActive()
{
	return bIsActive;
}
//--------------------------------------------------------------------------------------------------------------------------//
void APickupBase::setActive(bool NewPickupState)
{
	bIsActive = NewPickupState;
}
//--------------------------------------------------------------------------------------------------------------------------//
void APickupBase::OnGetCollected_Implementation()
{
	//�������� ������ �� �ʿ����ڵ� ����
	//��ƼŬ ���� ��....
}

