// (C) 2024- SAROS; 2021-2023 Gravitation; All Rights Reserved.


#include "WidgetStateActions.h"

#include "Animation/WidgetAnimation.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Components/Image.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(WidgetStateActions)

DEFINE_LOG_CATEGORY(LogWidgetStateAction)


void FActionPlaySingleAnim::DoBeforeChangeState(UUserWidget* Widget, FWidgetStateDefine const& FromState,
	FWidgetStateDefine const& ToState)
{
	AnimationNames.Empty(1);
	AnimationNames.Add(AnimName);
}



void FPlayLoopingAnim::DoWhenGotoState(UUserWidget* Widget, FWidgetStateDefine const& FromState,
	FWidgetStateDefine const& ToState)
{
	if (!IsValid(Widget))
	{
		return;
	}

	if (FWidgetStateManager const* Manager = FWidgetStateManager::GetStateManagerFromWidget(Widget);
		Manager && Manager->IsInterrupting())
	{
		// 正在过渡时调用了ActionDone，说明过渡动画被打断，此时不要播放动效
		return;
	}
	
	for (const FName& AnimName: AnimationNames)
	{
		if (UWidgetAnimation* Animation = GetAnimationByName(Widget, AnimName.ToString()))
		{
			Widget->PlayAnimation(Animation, 0, 0);
			
			// 缓存动画
			FWidgetStateTemporaryCache& Cache = GetStateManagerCache(*Widget);
			Cache.PlayingLoopAnimations.AddUnique(Animation);
			UE_LOG(LogWidgetStateAction, Verbose, TEXT("[FPlayLoopingAnim] Widget (%s) playing state(%s)'s loop animation:%s"),
				*Widget->GetName(), *ToState.ToString(), *AnimName.ToString()
				);
				
		}
	}
}

void FActionStopLoopAndPlayAnim::DoBeforeChangeState(UUserWidget* Widget, FWidgetStateDefine const& FromState,
												   FWidgetStateDefine const& ToState)
{
	if (!IsValid(Widget))
	{
		return;
	}
	FDefaultActionTransitionGoing::DoBeforeChangeState(Widget, FromState, ToState);
	
	FWidgetStateTemporaryCache& Cache = GetStateManagerCache(*Widget);
	for (UWidgetAnimation* Anim: Cache.PlayingLoopAnimations)
	{
		Widget->StopAnimation(Anim);
	}
	Cache.PlayingLoopAnimations.Empty();
	
}


void FActionExecuteBPEvent::DoWhenGotoState(UUserWidget* Widget, FWidgetStateDefine const& FromState,
                                            FWidgetStateDefine const& ToState)
{
	FWidgetStateActionTransitionDone::DoWhenGotoState(Widget, FromState, ToState);

	if (!IsValid(Widget))
	{
		return;
	}

	if (UFunction* Function = Widget->FindFunction(EventName))
	{
		UE_LOG(LogWidgetStateAction, Display, TEXT("FActionExecuteBPEvent found function %s"), *EventName.ToString());
		Widget->ProcessEvent(Function, nullptr);
	}
}



void FSingleWidgetSingleEntranceActionBase::DoWhenGotoState(UUserWidget* Widget, FWidgetStateDefine const& FromState,
	FWidgetStateDefine const& ToState)
{
	UE_LOG(LogWidgetStateAction, Log, TEXT("[SingleWidgetSingleEntranceActionBase] State changed to %s (widget %s)"),
		*ToState.ToString(), *GetNameSafe(Widget));
}

void FActionResetWidget::DoWhenGotoState(UUserWidget* Widget, FWidgetStateDefine const& FromState,
	FWidgetStateDefine const& ToState)
{
	Reset(Widget);
}

void FActionSetChildrenVisibility::DoWhenGotoState(UUserWidget* Widget, FWidgetStateDefine const& FromState,
                                                   FWidgetStateDefine const& ToState)
{
	if (!IsValid(Widget))
	{
		return;
	}
	// 批量设置可见性
	for (const auto&[WidgetName, bShow]: ChildrenVisibilityMap)
	{
	    if (UWidget* Child = Widget->GetWidgetFromName(WidgetName)) // todo 如果有性能消耗，override这个函数
	    {
	        if (bShow)
	        {
	            Child->SetVisibility(ESlateVisibility::HitTestInvisible);
	            UE_LOG(LogWidgetStateAction, Verbose, TEXT("[FActionSetChildrenVisibility] Set %s to HitTestInvisible"), *Child->GetName())
	        }
	        else 
	        {
	            Child->SetVisibility(ESlateVisibility::Collapsed);
	            UE_LOG(LogWidgetStateAction, Verbose, TEXT("[FActionSetChildrenVisibility] Set %s to Collapsed"), *Child->GetName())
	        }
	    }
	}
}

void FActionSetImageColor::DoWhenGotoState(UUserWidget* Widget, FWidgetStateDefine const& FromState,
	FWidgetStateDefine const& ToState)
{
	if (!IsValid(Widget))
	{
		return;
	}
	// 批量设置可见性
	for (const auto&[WidgetName, Color]: ColorMap)
	{
		if (UImage* Child = Cast<UImage>(Widget->GetWidgetFromName(WidgetName))) // todo 如果有性能消耗，修改这个函数进行Cache
		{
			Child->SetColorAndOpacity(Color);
			UE_LOG(LogWidgetStateAction, Verbose, TEXT("[FActionSetImageColor] Set %s to Color %s"), *Child->GetName(), *Color.ToString())
			
		}
		else 
		{
			UE_LOG(LogWidgetStateAction, Warning, TEXT("[FActionSetImageColor] Cannot find child %s in widget %s"), *WidgetName.ToString(), *Widget->GetName())
		}
	}
}

