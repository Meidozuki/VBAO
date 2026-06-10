// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetStateTypes.h"
#include "UObject/Interface.h"
#include "StatefulUMGInterface.generated.h"

class UWidgetStateDataAsset;
struct FWidgetStateManager;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class UStatefulUMG : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VBAOVIEWMODEL_API IStatefulUMG
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UWidgetStateDataAsset const* GetWidgetStateDataAsset() const = 0;
	virtual FWidgetStateManager& GetWidgetStateManager() = 0;
	
	UFUNCTION(BlueprintCallable)
	virtual void InitializeWidgetState(FName InitState);
	UFUNCTION(BlueprintCallable)
	virtual void ChangeWidgetState(FName NewState, bool bToPlayAnimation=true);
	
	FWidgetStateDefine GetCurrentWidgetState() const;
	
private:
	FWidgetStateManager const& GetWidgetStateManager() const;
};
