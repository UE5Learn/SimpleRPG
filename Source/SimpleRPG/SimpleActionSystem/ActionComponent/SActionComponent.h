// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"

class USAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged,USActionComponent*,OwnerActionComponent,USAction*,Action);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGameplayTagCountChanged,const FGameplayTag,Tag,int32,Count);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameplayTagCountChanged_M,const FGameplayTag,Tag,int32,Count);


UENUM(BlueprintType)
enum class EGameplayTagDelegateType : uint8
{
	NewOrRemoved			UMETA(DisplayName="生成或销毁"),
	AnyCountChange			UMETA(DisplayName="任何数量改变")
};

USTRUCT(BlueprintType)
struct FActionArray
{
	GENERATED_BODY()

	TArray<USAction*> Actions;
};

USTRUCT(BlueprintType)
struct FDelegateInfo
{
	GENERATED_BODY()

	FOnGameplayTagCountChanged_M OnNewOrRemove;
	FOnGameplayTagCountChanged_M OnAnyChange;
	
};


UCLASS( editinlinenew,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLERPG_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USActionComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*
	 *		Getter
	 */

	UFUNCTION(BlueprintPure,Category = "Tags")
	FGameplayTagContainer& GetActiveTagContainer();

	UFUNCTION(BlueprintPure,Category = "Tags")
	TMap<FGameplayTag,FActionArray>& GetActionTriggerTags();

	/*
	 *		Action处理
	 */

	/* 通过Class添加Action */
	UFUNCTION(BlueprintCallable,Category = "ActionComponent")
	void AddActionByClass(TSubclassOf<USAction> ActionClass);

	/* 通过Object添加Action */
	UFUNCTION(BlueprintCallable,Category = "ActionComponent")
	void AddActionByObject(USAction* ActionObject);

	void AddAction(USAction* ActionObject);

	/* 通过class激活Action */
	UFUNCTION(BlueprintCallable,Category = "ActionComponent")
	void TryActiveActionByClass(TSubclassOf<USAction> ActionClass);

	/* 通过Object激活Action */
	UFUNCTION(BlueprintCallable,Category = "ActionComponent")
	void TryActiveActionByObject(USAction* ActionObject);

	/*
	 *		Delegate处理
	 */
	
	/* 增加监听Tag的委托事件 */
	UFUNCTION(BlueprintCallable,Category = "ActionComponent")
	void AddListenerTagEvent(const FGameplayTag Tag,EGameplayTagDelegateType DelegateInfoType,const FOnGameplayTagCountChanged& Event);


	/*
	 *		Tags处理
	 */
	
	/* 添加Tags */
	void AppendTags(FGameplayTagContainer const& Other);

	/* 删除Tags */
	void RemoveTags(FGameplayTagContainer const& Other);
	
	

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionActive;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionEnd;

protected:
	/* 被激活存在的标签 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;
	
	/* 根据标签触发激活的Action */
	UPROPERTY(BlueprintReadWrite,Category = "Tags")
	TMap<FGameplayTag,FActionArray> ActionTriggerTags;

	/* 被添加到自身的Action */
	UPROPERTY(BlueprintReadOnly)
	TMap<FName,TObjectPtr<USAction>> Actions;
	
	/* 每个标签的对应的数量 */
	UPROPERTY(BlueprintReadOnly,Category = "Tags")
	TMap<FGameplayTag,int32> GameplayTagCountMap;

	/* 每个标签对应的委托事件 */
	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag,FDelegateInfo> GameplayTagEventMap;

	

		
};
