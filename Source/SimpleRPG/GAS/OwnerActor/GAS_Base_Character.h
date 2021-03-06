// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "SimpleRPG/GAS/AttributeSet/BaseAttributeSet.h"
#include "SimpleRPG/GAS/Component/RPG_ASC.h"
#include "GAS_Base_Character.generated.h"

UCLASS()
class SIMPLERPG_API AGAS_Base_Character : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGAS_Base_Character();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	/* AbilitySystemComponent */
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<URPG_ASC> RPG_AbilitySystemComponent;
	
	/* 尝试获取Attribute CurrentValue，如果未拥有该Attribute，返回值为0 */
	UFUNCTION(BlueprintPure,Category = "GAS|Attributes")
	float GetAttributeValue(FGameplayAttribute Attribute, bool& bSuccessfullyFoundAttribute) const;

	/* 尝试获取Attribute BaseValue，如果未拥有该Attribute，返回值为0 */
	UFUNCTION(BlueprintPure,Category = "GAS|Attributes")
	float GetAttributeBaseValue(FGameplayAttribute Attribute, bool& bSuccessfullyFoundAttribute) const;
	
	void OnAttributeValueChange(const FOnAttributeChangeData& Data);
	/* 如果监听了AttributeChange事件,所有AttributeValue被改变时都会调用该事件通知蓝图，传入被改变的AttributeName和NewValue,OldValue */
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS|K2Node")
	void K2_OnAttributeValueChange(const FString& Name,float NewValue,float OldValue);

	/* 被添加到ListenNewOrRemoveTags数组中的tag被生成或被完全删除时调用该事件 */
	UFUNCTION(BlueprintImplementableEvent,Category = "GAS|K2Node")
	void K2_OnTagNewOrRemove(const FGameplayTag GameplayTag,int32 Count);

	/* 被添加到ListenAnyCountChangeTags数组的tagCount被改变时调用该事件 */
	UFUNCTION(BlueprintImplementableEvent,Category = "GAS|K2Node")
	void K2_OnTagCountChange(const FGameplayTag GameplayTag,int32 Count);
	
	

	



protected:

	/* 是否需要注册监听所有AttributeValueChange事件 */
	UPROPERTY(EditAnywhere,Category = "GAS_Base_Character|DefaultValue")
	bool ShouldRegisterAllAttributeChangeEvent;

	/* 该数组的tags的第一次添加和被完全删除事件被监听 */
	UPROPERTY(EditAnywhere,Category = "GAS_Base_Character|Tags")
	TArray<FGameplayTag> ListenNewOrRemoveTags;

	/* 该数组的tags任何TagCount被改变事件被监听 */
	UPROPERTY(EditAnywhere,Category = "GAS_Base_Character|Tags")
	TArray<FGameplayTag> ListenAnyCountChangeTags;

	/* AttributeSet数组,在BeginPlay初始化 */
	UPROPERTY(EditAnywhere,Category = "GAS_Base_Character|Abilitys")
	TArray<TSubclassOf<UAttributeSet>> AttributeSets;

	/* Ability数组,在BeginPlay添加到AbilitySystemCOmponent */
	UPROPERTY(EditAnywhere,Category = "GAS_Base_Character|Abilitys")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;

	/* 初始化GE,会在AttributeSet初始化后用于初始化AttributeSet的初始值，该GE应只用于设置初值 */
	UPROPERTY(EditAnywhere,Category = "GAS_Base_Character|Abilitys")
	TSubclassOf<UGameplayEffect> InitializeEffect;

	
	

};
