// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyAIController.h"
#include "Gladiator.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/SkeletalMeshComponent.h"
#include "CollisionQueryParams.h"
#include "GameFramework/DamageType.h"
#include "Engine/Engine.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AEnemyAIController::StaticClass();

	TotalHealth = 100.f;
	AttackRange = 100.f;
	AttackDamage = 10.f;
	IsDead = false;
	IsAttacking = false;

	bUseControllerRotationYaw = true;

	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensor"));
	PawnSensor->SensingInterval = .25f;	//초당 4번
	PawnSensor->bOnlySensePlayers = true;
	PawnSensor->SetPeripheralVisionAngle(85.f);
	
	bodySphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("BodyTriggerSphere"));
	bodySphereTrigger->SetSphereRadius(150.f);
	
	bodySphereTrigger->SetupAttachment(GetMesh());

	FColor handsTriggersSolor = FColor(0, 0, 255, 255);
	leftHandTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandTriggerSphere"));
	leftHandTrigger->SetSphereRadius(70.f);
	leftHandTrigger->SetupAttachment(GetMesh(), TEXT("hand_lf"));
	leftHandTrigger->ShapeColor = handsTriggersSolor;
	leftHandTrigger->SetGenerateOverlapEvents(false);

	rightHandTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandTriggerSphere"));
	rightHandTrigger->SetSphereRadius(70.f);
	rightHandTrigger->SetupAttachment(GetMesh(), TEXT("hand_rt"));
	rightHandTrigger->ShapeColor = handsTriggersSolor;
	rightHandTrigger->SetGenerateOverlapEvents(false);

	GetCharacterMovement()->NavAgentProps.AgentRadius = 400.f;	
	GetCharacterMovement()->SetUpdateNavAgentWithOwnersCollisions(false);

}
//--------------------------------------------------------------------------------------------------------------------------//
void AEnemy::PostInitializeComponents()
{
	//BeginPlay 보다 먼저 호출한다.
	Super::PostInitializeComponents();
	PawnSensor->OnSeePawn.AddDynamic(this, &AEnemy::OnSeePawn);
	PawnSensor->OnHearNoise.AddDynamic(this, &AEnemy::OnHearNoise);

	leftHandTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnHandTriggerOverlap);
	rightHandTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnHandTriggerOverlap);
}
//--------------------------------------------------------------------------------------------------------------------------//
// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}
//--------------------------------------------------------------------------------------------------------------------------//
// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//--------------------------------------------------------------------------------------------------------------------------//
// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
//--------------------------------------------------------------------------------------------------------------------------//
void AEnemy::OnPerformAttack()
{
	const FVector TraceStart = GetActorLocation();
	const FVector LookingDirection = GetActorRotation().Vector();
	const FVector TraceEnd = TraceStart + LookingDirection * AttackRange;

	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));
	FCollisionQueryParams TracingParameters(WeaponFireTag, true);
	TracingParameters.AddIgnoredActor(this);
	TracingParameters.bTraceComplex = true;
	TracingParameters.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	
	GetWorld()->SweepSingleByChannel(Hit, TraceStart, TraceEnd, FQuat::Identity, ECollisionChannel::ECC_Pawn, FCollisionShape::MakeSphere(25), TracingParameters); //폰하고만 충돌한다.
	
	if (nullptr != Hit.Actor)
	{
		ACharacter* character = Cast<ACharacter>(Hit.Actor);
		if (nullptr != character)
		{
			FPointDamageEvent	PointDmg;
			PointDmg.DamageTypeClass = UDamageType::StaticClass();
			PointDmg.HitInfo = Hit;
			PointDmg.ShotDirection = LookingDirection;
			PointDmg.Damage = AttackDamage;
			character->TakeDamage(AttackDamage, PointDmg, Controller, this);

			IsAttacking = true;
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------//
void AEnemy::OnPreAttack()
{
	leftHandTrigger->SetGenerateOverlapEvents(true);
	rightHandTrigger->SetGenerateOverlapEvents(true);

	//FString message = TEXT(">>>>>>>>>>>> Attack just going to start <<<<<<<<<<<<<<<<<<");
}
//--------------------------------------------------------------------------------------------------------------------------//
void AEnemy::OnPostAttack()
{
	IsAttacking = false;
	leftHandTrigger->SetGenerateOverlapEvents(false);
	rightHandTrigger->SetGenerateOverlapEvents(false);
}
//--------------------------------------------------------------------------------------------------------------------------//
void AEnemy::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{

}
//--------------------------------------------------------------------------------------------------------------------------//
void AEnemy::OnSeePawn(APawn* OtherPawn)
{

}
//--------------------------------------------------------------------------------------------------------------------------//
void AEnemy::OnHandTriggerOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherCOmp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGladiator* const _tempGladiator = Cast<AGladiator>(OtherActor);
	if (nullptr != _tempGladiator)
	{
		FString message = TEXT("=== HIT PLAYER WITH HAND ==== ");
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, message);
		leftHandTrigger->SetGenerateOverlapEvents(false);
		rightHandTrigger->SetGenerateOverlapEvents(false);

		_tempGladiator->OnChangeHealthByAmount(5.f);
	}
}

