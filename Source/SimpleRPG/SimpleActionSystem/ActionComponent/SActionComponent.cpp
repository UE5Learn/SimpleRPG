// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"
#include "SimpleRPG/SimpleActionSystem/Action/SAction.h"


USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;


}



void USActionComponent::BeginPlay()
{
	Super::BeginPlay();


	
}



void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

FGameplayTagContainer& USActionComponent::GetActiveTagContainer()
{
	return ActiveGameplayTags;
}

void USActionComponent::AddActionByClass(TSubclassOf<USAction> ActionClass)
{
	if(!ensure(ActionClass))
		return;
	
	
	if(Actions.Contains(ActionClass->GetFName()))
	{
		UE_LOG(LogTemp,Warning,TEXT("[Class: %s] 已经存在"),*GetNameSafe(ActionClass));
		return;
	}

	TObjectPtr<USAction> NewAction = NewObject<USAction>(GetOwner(),ActionClass);
	if(ensure(NewAction))
	{
		NewAction->Initialize(this,this);
		Actions.Add(NewAction.GetClass()->GetFName(),NewAction);
	}
	
}

void USActionComponent::AddActionByObject(USAction* ActionObject)
{
	if(!ensure(ActionObject))
		return;

	if(Actions.Contains(ActionObject->GetClass()->GetFName()))
	{
		UE_LOG(LogTemp,Warning,TEXT("[Class: %s] 已经存在"),*GetNameSafe(ActionObject->GetClass()));
		return;
	}

	ActionObject->Initialize(this,this);
	Actions.Add(ActionObject->GetClass()->GetFName(),ActionObject);
}

