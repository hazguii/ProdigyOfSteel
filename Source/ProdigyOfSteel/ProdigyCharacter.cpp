// Fill out your copyright notice in the Description page of Project Settings.


#include "ProdigyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AProdigyCharacter::AProdigyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and setup the spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;

	// Create and setup the camera component
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Set the character to use controller rotation for movement
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Allow the character to rotate with the camera
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

// Called when the game starts or when spawned
void AProdigyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProdigyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AProdigyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind movement functions
	PlayerInputComponent->BindAxis("MoveForward", this, &AProdigyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProdigyCharacter::MoveRight);
	
	// Bind jump function
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AProdigyCharacter::Jump);
	
	// Bind mouse look functions
	PlayerInputComponent->BindAxis("Turn", this, &AProdigyCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AProdigyCharacter::LookUp);
}

void AProdigyCharacter::MoveForward(float Value)
{
	if (Controller && Value != 0.0f)
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AProdigyCharacter::MoveRight(float Value)
{
	if (Controller && Value != 0.0f)
	{
		// Find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AProdigyCharacter::Jump()
{
	// Call the parent class jump function
	Super::Jump();
}

void AProdigyCharacter::Turn(float Value)
{
	if (Value != 0.0f)
	{
		// Add yaw rotation (left/right turn) with sensitivity
		AddControllerYawInput(Value * TurnSensitivity);
	}
}

void AProdigyCharacter::LookUp(float Value)
{
	if (Value != 0.0f)
	{
		// Add pitch rotation (up/down look) with sensitivity
		AddControllerPitchInput(Value * LookUpSensitivity);
	}
}

