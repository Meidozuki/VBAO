// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatefulUMGInterface.h"
#include "WidgetStateManager.h"
#include "StatefulUserWidget.generated.h"

class UWidgetStateDataAsset;

/**
 * 
 */
UCLASS()
class VBAOVIEWMODEL_API UStatefulUserWidget : public UUserWidget, public IStatefulUMG
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="Widget State")
	TObjectPtr<UWidgetStateDataAsset> WidgetStateDataAsset;

	FWidgetStateManager WidgetStateManager;

public:
	// IStatefulUMG interface
	virtual UWidgetStateDataAsset const* GetWidgetStateDataAsset() const override;
	virtual FWidgetStateManager& GetWidgetStateManager() override;
	
	// 暴露给蓝图，方便debug用，请勿用于业务逻辑
	UFUNCTION(BlueprintCallable, meta=(DevelopmentOnly))
	FName BP_GetCurrentWidgetStateName() const { return GetCurrentWidgetState().ToFName();}
	
	
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
