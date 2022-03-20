// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"

ARPG_PlayerController::ARPG_PlayerController()
{
}

void ARPG_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(DefaultInputMappingContext)
	{
		if(UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(DefaultInputMappingContext,0);
		}

		if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
		{
			if(IA_MoveForward)
				EnhancedInputComponent->BindAction(IA_MoveForward,ETriggerEvent::Triggered,this,&ARPG_PlayerController::MoveForward);

			if(IA_MoveRight)
				EnhancedInputComponent->BindAction(IA_MoveRight,ETriggerEvent::Triggered,this,&ARPG_PlayerController::MoveRight);

			if(IA_LookUp)
				EnhancedInputComponent->BindAction(IA_LookUp,ETriggerEvent::Triggered,this,&ARPG_PlayerController::LookUp);

			if(IA_Turn)
				EnhancedInputComponent->BindAction(IA_Turn,ETriggerEvent::Triggered,this,&ARPG_PlayerController::Turn);
		}
	}
}

void ARPG_PlayerController::MoveForward(const FInputActionValue& InputActionValue)
{
	const FRotator YawRotator = FRotator(0,GetControlRotation().Yaw,0);
	const FVector Direction = UKismetMathLibrary::GetForwardVector(YawRotator);
	GetPawn()->AddMovementInput(Direction,InputActionValue.GetMagnitude());
}

void ARPG_PlayerController::MoveRight(const FInputActionValue& InputActionValue)
{
	const FRotator YawRotator = FRotator(0,GetControlRotation().Yaw,0);
	const FVector Direction = UKismetMathLibrary::GetRightVector(YawRotator);
	GetPawn()->AddMovementInput(Direction,InputActionValue.GetMagnitude());
}

void ARPG_PlayerController::LookUp(const FInputActionValue& InputActionValue)
{
	GetPawn()->AddControllerPitchInput(InputActionValue.GetMagnitude());
}

void ARPG_PlayerController::Turn(const FInputActionValue& InputActionValue)
{
	GetPawn()->AddControllerYawInput(InputActionValue.GetMagnitude());
}
