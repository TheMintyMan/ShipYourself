// Fill out your copyright notice in the Description page of Project Settings.

#include "SYCharacter.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimMontage.h"

// Sets default values
ASYCharacter::ASYCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->Crouch();

	bUseControllerRotationYaw = false;
	
	
	
}

void ASYCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

// Called when the game starts or when spawned
void ASYCharacter::BeginPlay()
{
	Super::BeginPlay();
	Crouch();
	
	
	// Set up the input mapping context system
	if(const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			InputSystem->AddMappingContext(InputMapping, 0);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Crouch: %d"), bIsCrouched);
}

void ASYCharacter::Move(const FInputActionValue& Value)
{
	const FVector2d DirectionValue = Value.Get<FVector2d>();

	// Getting the rotation of the controller and setting the yaw
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	// Move Forward
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, DirectionValue.Y);
		
	// Move Right
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, DirectionValue.X);
	
}

void ASYCharacter::Look(const FInputActionValue& Value)
{
	const FVector2d LookAxisValue = Value.Get<FVector2d>();
	
	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void ASYCharacter::CrouchChar(const FInputActionValue& Value)
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
		
	UE_LOG(LogTemp, Warning, TEXT("Crouch: %d"), bIsCrouched);
}

// Called to bind functionality to input
void ASYCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction , ETriggerEvent::Triggered, this, &ASYCharacter::Move);
		Input->BindAction(LookAction , ETriggerEvent::Triggered, this, &ASYCharacter::Look);
		Input->BindAction(JumpAction , ETriggerEvent::Triggered, this, &ASYCharacter::Jump);
		Input->BindAction(ToggleCrouchAction , ETriggerEvent::Triggered, this, &ASYCharacter::CrouchChar);
		
	}

}

