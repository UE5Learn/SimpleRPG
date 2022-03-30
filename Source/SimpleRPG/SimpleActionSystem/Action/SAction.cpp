// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SimpleRPG/SimpleActionSystem/ActionComponent/SActionComponent.h"

void USAction::Initialize(TObjectPtr<USActionComponent> InstigatorActionComp,TObjectPtr<USActionComponent> OwnerActionComp)
{
	if(InstigatorActionComp && OwnerActionComp)
	{
		ActionSpec.Instigator = InstigatorActionComp;
		ActionSpec.Owner = OwnerActionComp;
		if(TriggerTag.IsValid())
		{
			ActionSpec.Owner->GetActionTriggerTags().FindOrAdd(TriggerTag).Actions.Add(this);
		}
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

void USAction::ActiveRunning()
{
	if(CooldownSpec.CooldownType == ECooldownType::Default)
	{
		ActionSpec.bIsRunning = true;
		return;
	}
	if(CooldownSpec.CooldownTime > 0.0f)
	{
		CurrentCooldownTime = CooldownSpec.CooldownTime;
		ActionSpec.bIsRunning = true;
		GetWorld()->GetTimerManager().SetTimer(CooldownHandle,this,&USAction::CheckActionCooldown,CooldownSpec.Rate,true,0);
	}
}

void USAction::EndRunning()
{
	if(CooldownSpec.CooldownType == ECooldownType::Default)
	{
		ActionSpec.bIsRunning = false;
	}
}

void USAction::CheckActionCooldown()
{
	CurrentCooldownTime  = CurrentCooldownTime - CooldownSpec.Rate;
	if(CurrentCooldownTime <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownHandle);
		ActionSpec.bIsRunning = false;
	}
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
	if(!OnCanActive())
		return false;
	if(IsRunning())
		return false;
	
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

	TObjectPtr<USActionComponent> OwnerActionComponent = GetOwnerActionComponent();
	OwnerActionComponent->AppendTags(ApplyToOwnerTags);
	ActiveRunning();
	OwnerActionComponent->OnActionActive.Broadcast(OwnerActionComponent,this);
	OnActiveAction();
}

void USAction::EndAction()
{
	TObjectPtr<USActionComponent> OwnerActionComponent = GetOwnerActionComponent();
	OwnerActionComponent->RemoveTags(ApplyToOwnerTags);
	EndRunning();
	OwnerActionComponent->OnActionEnd.Broadcast(OwnerActionComponent,this);
	OnEndAction();
}
