// Fill out your copyright notice in the Description page of Project Settings.


#include "StatefulUMGInterface.h"

#include "WidgetStateManager.h"
#include "Blueprint/UserWidget.h"


void IStatefulUMG::InitializeWidgetState(FName InitState)
{
	FWidgetStateDefine State(InitState);

	GetWidgetStateManager().Initialize(Cast<UUserWidget>(this), State);
}

void IStatefulUMG::ChangeWidgetState(FName NewState, bool bToPlayAnimation)
{
	FWidgetStateDefine State(NewState);
	
	// 快速切换的边界情况，不能在这里判断From/To State是否相等，调进去再处理
	GetWidgetStateManager().ChangeWidgetState(Cast<UUserWidget>(this), State, bToPlayAnimation);
}

FWidgetStateDefine IStatefulUMG::GetCurrentWidgetState() const
{
	return GetWidgetStateManager().GetCurrentDesiringWidgetState();
}

FWidgetStateManager const& IStatefulUMG::GetWidgetStateManager() const
{
	return const_cast<IStatefulUMG*>(this)->GetWidgetStateManager();
}
