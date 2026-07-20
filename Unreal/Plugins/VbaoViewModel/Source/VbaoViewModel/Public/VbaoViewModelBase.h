// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"

#include <expected>

#include "VbaoViewModelBase.generated.h"

enum class EVbaoSetModelError
{
	KInvalidInputPointer,
	KViewModelClassMismatch,
};

USTRUCT()
struct VBAOVIEWMODEL_API FVbaoModelBase
{
	GENERATED_BODY()
};

/**
 * 
 */
UCLASS()
class VBAOVIEWMODEL_API UVbaoViewModelBase : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	// Set up a U-VM from FStruct data
	virtual void SetUpModel(const FVbaoModelBase& Model);
	// Do Property replacement with the input Model
	void SetModel(UMVVMViewModelBase* InModel);

	// InModel can be considered as *const*, except that when this->PropertyVM is nullptr, InModel will replace it.
	[[nodiscard]]
	auto SetModelChecked(UMVVMViewModelBase* InModel) -> std::expected<UMVVMViewModelBase*, EVbaoSetModelError>;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVMViewModelBase> PropertyViewModelClass = UMVVMViewModelBase::StaticClass();

	virtual void TakeNewData(const UMVVMViewModelBase* NewData);

private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<UMVVMViewModelBase> PropertyVM;

	// 纠结了很久应该叫State还是Context，如果整体比较封闭，像IOMonad一样封装的话更倾向于叫State
	// 但是这个VM是由Widget完全掌控的，相对不可控，还是叫做Context了
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<UMVVMViewModelBase> ContextVM;
};
