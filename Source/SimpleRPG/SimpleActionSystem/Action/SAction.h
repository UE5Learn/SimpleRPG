// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

class UWorld;
class USActionComponent;



UENUM(BlueprintType)
enum class EActionInstancingPolicy : uint8
{
	PerActor		UMETA(DisplayName="实例化一次"),
	PerExecution	UMETA(DisplayName="每次执行实例化"),
	None			UMETA(DisplayName="不实例化")
};

USTRUCT(BlueprintType)
struct FActionSpec
{
	GENERATED_BODY()

	/* Action的运行状态 */
	UPROPERTY(BlueprintReadWrite)
	bool bIsRunning;

	/* Action的激发者 */
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<USActionComponent> Instigator;

	/* Action的目标 */
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<USActionComponent> Owner;
	
};

UENUM(BlueprintType)
enum class ECooldownType : uint8
{
	Default			UMETA(DisplayName="默认"),
	Time			UMETA(DisplayName="时间")
};

USTRUCT(BlueprintType)
struct FCooldownSpec
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	ECooldownType CooldownType;

	UPROPERTY(EditDefaultsOnly,meta = (EditCondition = "CooldownType == ECooldownType::Time",EditConditionHides))
	float CooldownTime;

	float Rate = 0.1;
	
};



UCLASS(Blueprintable)
class SIMPLERPG_API USAction : public UObject
{
	GENERATED_BODY()

public:
	
	void Initialize(TObjectPtr<USActionComponent> InstigatorActionComp,TObjectPtr<USActionComponent> OwnerActionComp);
	
	/* 获取这个Action的OwnerActionComponent */
	UFUNCTION(BlueprintPure,Category = "Action")
	USActionComponent* GetOwnerActionComponent() const;

	

	/* 获取当前Action是否在运行 */
	UFUNCTION(BlueprintPure,Category = "Action")
	bool IsRunning() const;

	/* 激活运行状态 */
	void ActiveRunning();
	/* 检查冷却时间 */
	void CheckActionCooldown();

	/* 检查是否可以激活技能 */
	bool CanActive();
	
	/* 额外判定是否可以激活 */
	UFUNCTION(BlueprintNativeEvent,Category = "Action")
	bool OnCanActive();
	
	void ActiveAction();

	/* Action激活时调用 */
	UFUNCTION(BlueprintImplementableEvent,Category = "Action")
	void OnActiveAction();

	/* 结束Action */
	UFUNCTION(BlueprintCallable,Category = "Action")
	void EndAction();

	/* Action结束时调用 */
	UFUNCTION(BlueprintImplementableEvent,Category = "Action")
	void OnEndAction();

	virtual  UWorld* GetWorld() const override;

protected:

	/* 在被添加到ActionComponent时是否自动激活 */
	UPROPERTY(EditDefaultsOnly,Category = "Action")
	bool bAutoActive = false;

	/* 技能说明 */
	UPROPERTY(BlueprintReadOnly,Category = "Action")
	FActionSpec ActionSpec;

	UPROPERTY(EditDefaultsOnly,Category = "Action")
	FName ActionName;

	/* 激活时赋予Owner的Tags */
	UPROPERTY(EditDefaultsOnly,Category = "Tags")
	FGameplayTagContainer ApplyToOwnerTags;

	/* 目标没有这些tag才可以激活 */
	UPROPERTY(EditDefaultsOnly,Category = "Tags")
	FGameplayTagContainer BlockedTags;

	/* 该标签被添加时自动激活 */
	UPROPERTY(EditDefaultsOnly,Category = "Tags")
	FGameplayTag TriggerTag;

	/* 冷却时间设置 */
	UPROPERTY(EditDefaultsOnly,Category = "Cooldown")
	FCooldownSpec CooldownSpec;

	FTimerHandle CooldownHandle;

	float CurrentCooldownTime;
	
};
