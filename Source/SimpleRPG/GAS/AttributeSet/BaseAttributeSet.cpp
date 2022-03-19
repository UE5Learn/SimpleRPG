// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "SimpleRPG/GAS/OwnerActor/GAS_Base_Character.h"

UBaseAttributeSet::UBaseAttributeSet() :
	Health(100.0f),
	MaxHealth(100.0f),
	Stamina(100.0f),
	MaxStamina(100.0f),
	Mana(100.0f),
	MaxMana(100.0f)
{
	
}

void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	/* 限制CurrentValue的值在0和MaxValue之间 */
	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.0f,GetMaxHealth());
	}
	else if(Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.0f,GetMaxStamina());
	}
	else if(Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.0f,GetMaxMana());
	}

	/* MaxValue改变时，是否需要保持CurrentValue和MaxValue的百分比，如果需要保持，在下方添加AdjustAttributeForMaxChange函数 */
	if(Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health,MaxHealth,NewValue,GetHealthAttribute());
	}
	else if(Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana,MaxMana,NewValue,GetManaAttribute());
	}
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	
}

void UBaseAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* OwnerAbilitySystemComponent = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if(OwnerAbilitySystemComponent && !FMath::IsNearlyEqual(CurrentMaxValue,NewMaxValue))
	{
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		const float NewValue = (CurrentMaxValue > 0) ? (CurrentValue / CurrentMaxValue * NewMaxValue) : NewMaxValue;
		OwnerAbilitySystemComponent->ApplyModToAttribute(AffectedAttributeProperty,EGameplayModOp::Override,NewValue);
	}
}
