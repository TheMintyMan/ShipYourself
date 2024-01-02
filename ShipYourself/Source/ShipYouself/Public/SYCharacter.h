// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SYCharacter.generated.h"

class UInputAction;
struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;

UCLASS()
class SHIPYOUSELF_API ASYCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASYCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* InputMapping;
	
	// Movement Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* ToggleCrouchAction;
	
	// Movement Functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void CrouchChar(const FInputActionValue& Value);

	// Animations

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

private:
	bool bCheckIfCrouched;

	
};