// Fill out your copyright notice in the Description page of Project Settings.


#include "StatefulUserWidget.h"

UWidgetStateDataAsset const* UStatefulUserWidget::GetWidgetStateDataAsset() const
{
	return WidgetStateDataAsset;
}

FWidgetStateManager& UStatefulUserWidget::GetWidgetStateManager()
{
	return WidgetStateManager;
}

void UStatefulUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);

	if (WidgetStateManager.IsEnabled())
	{
		WidgetStateManager.TransitionAnimationFinishedCallback(this, Animation);
	}
}
