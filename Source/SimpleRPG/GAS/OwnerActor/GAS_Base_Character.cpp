// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_Base_Character.h"

#include "Kismet/GameplayStatics.h"


AGAS_Base_Character::AGAS_Base_Character()
{
	PrimaryActorTick.bCanEverTick = false;
	ShouldRegisterAllAttributeChangeEvent = false;

	RPG_AbilitySystemComponent = CreateDefaultSubobject<URPG_ASC>(TEXT("RPG_ASC"));

}


void AGAS_Base_Character::BeginPlay()
{
	Super::BeginPlay();

	if(RPG_AbilitySystemComponent)
	{
		/* 初始化AttributeSet */
		for(const auto& AttributeSet : AttributeSets)
		{
			if(AttributeSet)
			{
				auto NewAttributeSet = Cast<UAttributeSet>(UGameplayStatics::SpawnObject(AttributeSet.Get(),this));
				if(NewAttributeSet)
					RPG_AbilitySystemComponent->AddAttributeSetSubobject(NewAttributeSet);
			}
		}

		/* 初始化Ability */
		for(const auto& Ability : Abilities)
		{
			if(Ability)
				RPG_AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability,1,0));
		}
		RPG_AbilitySystemComponent->InitAbilityActorInfo(this,this);

		/* 如果InitalizeEffect有效，则对自身添加该Effect */
		if(InitializeEffect)
		{
			const auto& GameplayEffectSpecHandle = RPG_AbilitySystemComponent->MakeOutgoingSpec(InitializeEffect,1,RPG_AbilitySystemComponent->MakeEffectContext());
			RPG_AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
		}


		if(ShouldRegisterAllAttributeChangeEvent)
		{
			/* 获取拥有的Attributes,监听AttributeValueChange事件 */
			TArray<FGameplayAttribute> WaitBindAttributes;
			RPG_AbilitySystemComponent->GetAllAttributes(WaitBindAttributes);
			for(const auto& Attribute : WaitBindAttributes)
			{
				if(Attribute.IsValid())
					RPG_AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this,&AGAS_Base_Character::OnAttributeValueChange);
			}
		}
		
		
	}
	
	
}


void AGAS_Base_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGAS_Base_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* AGAS_Base_Character::GetAbilitySystemComponent() const
{
	return RPG_AbilitySystemComponent;
}


void AGAS_Base_Character::OnAttributeValueChange(const FOnAttributeChangeData& Data)
{
	K2_OnAttributeValueChange(Data.Attribute.AttributeName,Data.NewValue,Data.OldValue);
}

float AGAS_Base_Character::GetAttributeValue(FGameplayAttribute Attribute, bool& bSuccessfullyFoundAttribute) const
{
	bSuccessfullyFoundAttribute = true;

	if (!RPG_AbilitySystemComponent || !RPG_AbilitySystemComponent->HasAttributeSetForAttribute(Attribute))
	{
		bSuccessfullyFoundAttribute = false;
		return 0.f;
	}

	const float Result = RPG_AbilitySystemComponent->GetNumericAttribute(Attribute);
	return Result;
}

float AGAS_Base_Character::GetAttributeBaseValue(FGameplayAttribute Attribute, bool& bSuccessfullyFoundAttribute) const
{
	bSuccessfullyFoundAttribute = true;

	if (!RPG_AbilitySystemComponent || !RPG_AbilitySystemComponent->HasAttributeSetForAttribute(Attribute))
	{
		bSuccessfullyFoundAttribute = false;
		return 0.f;
	}

	const float Result = RPG_AbilitySystemComponent->GetNumericAttributeBase(Attribute);
	return Result;
}


