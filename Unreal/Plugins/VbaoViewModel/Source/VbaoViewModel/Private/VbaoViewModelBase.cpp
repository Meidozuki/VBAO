// Fill out your copyright notice in the Description page of Project Settings.


#include "VbaoViewModelBase.h"

void UVbaoViewModelBase::SetUpModel(const FVbaoModelBase& Model)
{
	UMVVMViewModelBase* VM = NewObject<UMVVMViewModelBase>(this, PropertyViewModelClass);
	SetModel(VM);
}

void UVbaoViewModelBase::SetModel(UMVVMViewModelBase* InModel)
{
	auto Either = SetModelChecked(InModel);
	ensureAlways(Either.has_value());
}

auto UVbaoViewModelBase::SetModelChecked(
	UMVVMViewModelBase* InModel) -> std::expected<UMVVMViewModelBase*, EVbaoSetModelError>
{
	if (!IsValid(InModel))
	{
		return std::unexpected(EVbaoSetModelError::KInvalidInputPointer);
	}
	if (!InModel->IsA(PropertyViewModelClass))
	{
		return std::unexpected(EVbaoSetModelError::KViewModelClassMismatch);
	}

	if (PropertyVM == nullptr)
	{
		PropertyVM = InModel;
	}
	else
	{
		TakeNewData(InModel);
	}
	return PropertyVM;
}

void UVbaoViewModelBase::TakeNewData(const UMVVMViewModelBase* NewData)
{
}
