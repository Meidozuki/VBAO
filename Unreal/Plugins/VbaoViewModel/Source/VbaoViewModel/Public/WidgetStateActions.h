// (C) 2024- SAROS; 2021-2023 Gravitation; All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WidgetStateManager.h"
#include "WidgetStateTypes.h"
#include "UObject/Object.h"
#include "WidgetStateActions.generated.h"

struct FGenericBlackboardContainer;

DECLARE_LOG_CATEGORY_EXTERN(LogWidgetStateAction, Log, All);

USTRUCT(DisplayName="播放动效")
struct VBAOVIEWMODEL_API FDefaultActionTransitionGoing : public FWidgetStateActionTransitionGoing
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FName> AnimationNames;
};

USTRUCT(DisplayName="播放单个动效")
struct VBAOVIEWMODEL_API FActionPlaySingleAnim : public FDefaultActionTransitionGoing
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, DisplayName="动效名称")
	FName AnimName;
	// TODO 把这个类的Array标记为Editor不可修改

	virtual void DoBeforeChangeState(UUserWidget* Widget, FWidgetStateDefine const& FromState,
								   FWidgetStateDefine const& ToState) override;
};

USTRUCT(DisplayName="停止Loop并播放动效")
struct VBAOVIEWMODEL_API FActionStopLoopAndPlayAnim : public FDefaultActionTransitionGoing, public FExtendedStateManagerFeature
{
	GENERATED_BODY()

	virtual void DoBeforeChangeState(UUserWidget* Widget, FWidgetStateDefine const& FromState,
								   FWidgetStateDefine const& ToState) override;
};

USTRUCT()
struct VBAOVIEWMODEL_API FPlayLoopingAnim : public FWidgetStateActionTransitionDone, public FExtendedStateManagerFeature
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSet<FName> AnimationNames;

	virtual void DoWhenGotoState(UUserWidget* Widget, FWidgetStateDefine const& FromState,
								 FWidgetStateDefine const& ToState) override;
};

USTRUCT(DisplayName="执行蓝图事件")
struct FActionExecuteBPEvent : public FWidgetStateActionTransitionDone
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName EventName;

	virtual void DoWhenGotoState(UUserWidget* Widget, FWidgetStateDefine const& FromState,
	                             FWidgetStateDefine const& ToState) override;
};


// 单个Widget的所有State都通过同一个Action处理，Action内部分发逻辑，需要继承重写
USTRUCT()
struct VBAOVIEWMODEL_API FSingleWidgetSingleEntranceActionBase : public FWidgetStateActionTransitionDone
{
	GENERATED_BODY()

	virtual void DoWhenGotoState(UUserWidget* Widget, FWidgetStateDefine const& FromState,
	                             FWidgetStateDefine const& ToState) override;

	virtual void Dispatch(UUserWidget* Widget, FWidgetStateDefine const& ToState)
	{
	}

	virtual void Dispatch(UUserWidget* Widget, FWidgetStateDefine const& FromState, FWidgetStateDefine const& ToState)
	{
		Dispatch(Widget, ToState);
	}
};

USTRUCT()
struct VBAOVIEWMODEL_API FActionResetWidget : public FWidgetStateActionTransitionDone
{
	GENERATED_BODY()

	virtual void DoWhenGotoState(UUserWidget* Widget, FWidgetStateDefine const& FromState,
	                             FWidgetStateDefine const& ToState) override;

	virtual void Reset(UUserWidget* Widget)
	{
	}
};


USTRUCT(DisplayName="批量设置Widget显隐")
struct VBAOVIEWMODEL_API FActionSetChildrenVisibility : public FWidgetStateActionTransitionDone
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, DisplayName="子控件名字 & 是否显示")
	TMap<FName, bool> ChildrenVisibilityMap;

	virtual void DoWhenGotoState(UUserWidget* Widget, FWidgetStateDefine const& FromState,
	                             FWidgetStateDefine const& ToState) override;
};

USTRUCT(DisplayName="批量设置UImage颜色")
struct VBAOVIEWMODEL_API FActionSetImageColor : public FWidgetStateActionTransitionDone
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, DisplayName="子控件名字 & ColorAndOpacity")
	TMap<FName, FLinearColor> ColorMap;

	virtual void DoWhenGotoState(UUserWidget* Widget, FWidgetStateDefine const& FromState,
	                             FWidgetStateDefine const& ToState) override;
};
