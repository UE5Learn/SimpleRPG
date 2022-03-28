// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"

class USAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged,USActionComponent*,OwnerActionComponent,USAction*,Action);


UCLASS( editinlinenew,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLERPG_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USActionComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure,Category = "Tags")
	FGameplayTagContainer& GetActiveTagContainer();

	UFUNCTION(BlueprintCallable,Category = "Action")
	void AddActionByClass(TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable,Category = "Action")
	void AddActionByObject(USAction* ActionObject);
	
	

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionActive;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionEnd;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UPROPERTY(BlueprintReadOnly)
	TMap<FName,TObjectPtr<USAction>> Actions;
	

		
};
