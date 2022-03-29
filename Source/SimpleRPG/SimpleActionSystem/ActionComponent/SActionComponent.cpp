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

TMap<FGameplayTag,FActionArray>& USActionComponent::GetActionTriggerTags()
{
	return ActionTriggerTags;
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
		AddAction(NewAction);
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
	AddAction(ActionObject);
}

void USActionComponent::AddAction(USAction* ActionObject)
{
	ActionObject->Initialize(this,this);
	Actions.Add(ActionObject->GetClass()->GetFName(),ActionObject);
}

void USActionComponent::TryActiveActionByClass(TSubclassOf<USAction> ActionClass)
{
	if(const auto& Action = Actions.Find(ActionClass->GetFName()))
	{
		Action->Get()->ActiveAction();
	}
}

void USActionComponent::TryActiveActionByObject(USAction* ActionObject)
{
	if(const auto& Action = Actions.Find(ActionObject->GetClass()->GetFName()))
	{
		Action->Get()->ActiveAction();
	}
}

void USActionComponent::AddListenerTagEvent(const FGameplayTag Tag, EGameplayTagDelegateType DelegateInfoType,
	const FOnGameplayTagCountChanged& Event)
{
	if(!Tag.IsValid())
	{
		return;
	}
	auto& DelegateInfo = GameplayTagEventMap.FindOrAdd(Tag);
	FScriptDelegate Delegate;
	Delegate.BindUFunction((UObject*)Event.GetUObject(),Event.GetFunctionName());
	if(DelegateInfoType == EGameplayTagDelegateType::AnyCountChange)
	{
		DelegateInfo.OnAnyChange.Add(Delegate);
	}
	else
	{
		DelegateInfo.OnNewOrRemove.Add(Delegate);
	}
}

void USActionComponent::AppendTags(FGameplayTagContainer const& Other)
{
	this->GetActiveTagContainer().AppendTags(Other);
	TArray<FGameplayTag> OtherTagArray;
	Other.GetGameplayTagArray(OtherTagArray);
	for(const FGameplayTag& OtherTag : OtherTagArray)
	{
		if(const auto& TagCount = GameplayTagCountMap.Find(OtherTag))
		{
			++*TagCount;
			if(const auto& Event = GameplayTagEventMap.Find(OtherTag))
			{
				Event->OnAnyChange.Broadcast(OtherTag,*TagCount);
			}
		}
		else
		{
			GameplayTagCountMap.Add(OtherTag,1);
			if(const auto& Event = GameplayTagEventMap.Find(OtherTag))
			{
				Event->OnNewOrRemove.Broadcast(OtherTag,1);
			}
		}

		if(const auto& ActionArray = ActionTriggerTags.Find(OtherTag))
		{
			for(const auto& Action : ActionArray->Actions)
			{
				Action->ActiveAction();
			} 
		}
	}
	
}

void USActionComponent::RemoveTags(FGameplayTagContainer const& Other)
{
	this->GetActiveTagContainer().RemoveTags(Other);
	TArray<FGameplayTag> OtherTagArray;
	Other.GetGameplayTagArray(OtherTagArray);
	for(const FGameplayTag& OtherTag : OtherTagArray)
	{
		if(const auto& TagCount = GameplayTagCountMap.Find(OtherTag))
		{
			--*TagCount;
			if(const auto& Event = GameplayTagEventMap.Find(OtherTag))
			{
				if(*TagCount == 0)
				{
					Event->OnNewOrRemove.Broadcast(OtherTag,0);
				}
				else
				{
					Event->OnAnyChange.Broadcast(OtherTag,*TagCount);
				}
			}
		}
	}
}

