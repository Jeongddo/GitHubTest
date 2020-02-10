// Fill out your copyright notice in the Description page of Project Settings.


#include "Gladiator.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "PaperSpriteComponent.h"
#include "Misc/OutputDeviceNull.h"
#include "GameFramework/Controller.h"
#include "PickupBase.h"
#include "CoinPickup.h"
#include "Engine/Engine.h"

// Sets default values
AGladiator::AGladiator()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	TotalHealth = 100.f;
	AttackRange = 25.f;
	jumppingVelocity = 600.f;

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = jumppingVelocity;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	EffectSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ClawEffect"));
	EffectSprite->AttachTo(CameraBoom);

	//NameTag = CreateDefaultSubobject<UWidget>(TEXT("Name"));
	//NameTag->AttachTo(RootComponent);
	

	IsStillAlive = true;
	IsAttacking = false;
	WeaponIndex = 1;

	OnSetPlayerController(true);

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
}
//--------------------------------------------------------------------------------------------------------------------------//
// Called when the game starts or when spawned
void AGladiator::BeginPlay()
{
	Super::BeginPlay();
	
}
//--------------------------------------------------------------------------------------------------------------------------//
void AGladiator::OnSetPlayerController(bool status)
{
	IsControlable = status;
}
//--------------------------------------------------------------------------------------------------------------------------//
void AGladiator::OnChangeHealthByAmount(float usedAmount)
{
	TotalHealth -= usedAmount;
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("ApplyGetDamageEffect"), ar, nullptr, true); //블루 프린트 함수 호출
}
//--------------------------------------------------------------------------------------------------------------------------//
void AGladiator::OnPosAttack()
{
		IsAttacking = false;
}
//--------------------------------------------------------------------------------------------------------------------------//
void AGladiator::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f) && IsControlable && !IsAttacking)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
//--------------------------------------------------------------------------------------------------------------------------//
void AGladiator::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f) && IsControlable && !IsAttacking)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}
//--------------------------------------------------------------------------------------------------------------------------//
void AGladiator::Jump()
{
	if (IsControlable && !IsAttacking)
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.f;
	}
}
//--------------------------------------------------------------------------------------------------------------------------//
void AGladiator::StopJumping()
{
	if (IsControlable)
	{
		bPressedJump = false;
		JumpKeyHoldTime = 0.f;
	}
}
//--------------------------------------------------------------------------------------------------------------------------//
void AGladiator::OnAttack()
{
	if (IsControlable)
	{
		IsAttacking = true;
	}
}
//--------------------------------------------------------------------------------------------------------------------------//
void AGladiator::OnChangeWeapon()
{
	if (IsControlable)
	{
		if (WeaponIndex < 1)
		{
			++WeaponIndex;
		}
		else
		{
			WeaponIndex = 1;
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------//
void AGladiator::OnCollectPickup()
{
	TArray<AActor*>	CollectedPicups;

	this->GetCapsuleComponent()->GetOverlappingActors(CollectedPicups);
	
	for (AActor* _tempPickup : CollectedPicups)
	{
		APickupBase* const pickup = Cast<APickupBase>(_tempPickup);
		if (nullptr != pickup && pickup->IsPendingKill() == false && pickup->IsActive() == true)
		{
			pickup->OnGetCollected();
			
			ACoinPickup* const _coin = Cast<ACoinPickup>(pickup);
			if (nullptr != _coin)
			{
				CollectedCoinsValue += _coin->getCoinValue();
				CollectedCoins++;

				FString message = TEXT("Collected another coinCoin ---> Total of ") + FString::FromInt(CollectedCoinsValue) ;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, message);
			}

			pickup->setActive(false);
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------//
void AGladiator::TurnAtRate(float Rate)
{
	if (IsControlable)
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}
//--------------------------------------------------------------------------------------------------------------------------//
void AGladiator::LookUpAtRate(float Rate)
{
	if (IsControlable)
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}
//--------------------------------------------------------------------------------------------------------------------------//
// Called every frame
void AGladiator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//--------------------------------------------------------------------------------------------------------------------------//
// Called to bind functionality to input
void AGladiator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AGladiator::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AGladiator::StopJumping);

	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AGladiator::OnAttack);

	PlayerInputComponent->BindAction("ChangeWeapon", IE_Released, this, &AGladiator::OnChangeWeapon);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGladiator::MoveForward);

	PlayerInputComponent->BindAxis("MoveRight", this, &AGladiator::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAxis("TurnRate", this, &AGladiator::TurnAtRate);

	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("LookUpRate", this, &AGladiator::LookUpAtRate);

	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

