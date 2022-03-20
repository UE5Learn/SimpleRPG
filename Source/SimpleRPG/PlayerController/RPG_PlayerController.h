// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "RPG_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API ARPG_PlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	ARPG_PlayerController();
	virtual void BeginPlay() override;

	UFUNCTION()
	void MoveForward(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void MoveRight(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void LookUp(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Turn(const FInputActionValue& InputActionValue);


protected:
	UPROPERTY(EditAnywhere,Category = "Input|MappingContext")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

	UPROPERTY(EditAnywhere,Category = "Input|Action")
	TObjectPtr<UInputAction> IA_MoveForward;

	UPROPERTY(EditAnywhere,Category = "Input|Action")
	TObjectPtr<UInputAction> IA_MoveRight;

	UPROPERTY(EditAnywhere,Category = "Input|Action")
	TObjectPtr<UInputAction> IA_LookUp;

	UPROPERTY(EditAnywhere,Category = "Input|Action")
	TObjectPtr<UInputAction> IA_Turn;
};
