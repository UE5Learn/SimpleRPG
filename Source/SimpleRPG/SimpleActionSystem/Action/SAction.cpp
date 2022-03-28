// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SimpleRPG/SimpleActionSystem/ActionComponent/SActionComponent.h"

void USAction::Initialize(TObjectPtr<USActionComponent> InstigatorActionComp,TObjectPtr<USActionComponent> OwnerActionComp)
{
	if(InstigatorActionComp && OwnerActionComp)
	{
		ActionSpec.Instigator = InstigatorActionComp;
		ActionSpec.Owner = OwnerActionComp;
		if(bAutoActive)
		{
			ActiveAction();
		}
	}
}

USActionComponent* USAction::GetOwnerActionComponent() const
{
	if(ActionSpec.Owner)
		return ActionSpec.Owner;
	return nullptr;
}

bool USAction::IsRunning() const
{
	return ActionSpec.bIsRunning;
}

UWorld* USAction::GetWorld() const
{
	TObjectPtr<AActor> Actor = Cast<AActor>(GetOuter());
	if(Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

bool USAction::CanActive()
{
	if(!OnCanActive() || IsRunning())
	{
		return false;
	}
	
	TObjectPtr<USActionComponent> OwnerActionComponent = GetOwnerActionComponent();
	if(!OwnerActionComponent || OwnerActionComponent->GetActiveTagContainer().HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

bool USAction::OnCanActive_Implementation()
{
	return true;
}

void USAction::ActiveAction()
{
	if(!CanActive())
	{
		return;
	}
	UE_LOG(LogTemp,Log,TEXT("ActionStarted: %s"),*GetNameSafe(this));

	TObjectPtr<USActionComponent> OwnerActionComponent = GetOwnerActionComponent();
	OwnerActionComponent->GetActiveTagContainer().AppendTags(ApplyToOwnerTags);
	ActionSpec.bIsRunning = true;
	OwnerActionComponent->OnActionActive.Broadcast(OwnerActionComponent,this);
	OnActiveAction();
}

void USAction::EndAction()
{
	UE_LOG(LogTemp,Log,TEXT("ActionEnd: %s"),*GetNameSafe(this));

	TObjectPtr<USActionComponent> OwnerActionComponent = GetOwnerActionComponent();
	OwnerActionComponent->GetActiveTagContainer().RemoveTags(ApplyToOwnerTags);
	ActionSpec.bIsRunning = false;
	OwnerActionComponent->OnActionEnd.Broadcast(OwnerActionComponent,this);
}
