// (C) 2024- SAROS; 2021-2023 Gravitation; All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WidgetStateTypes.h"
#include "StructUtils/InstancedStructContainer.h"
#include "UObject/Object.h"
#include "WidgetStateManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWidgetStateManager, Log, All)

class UWidgetAnimation;


UWidgetAnimation* GetAnimationByName(UUserWidget* Widget, const FString& InAnimKey);


USTRUCT()
struct FWidgetStateTemporaryCache
{
	GENERATED_BODY()
	
	UPROPERTY(Transient)
	TArray<TObjectPtr<UWidgetAnimation>> PlayingLoopAnimations;
};

// 操作StateManager的Cache的类都继承这个，表示是为了使用方便而拓展的功能
struct VBAOVIEWMODEL_API FExtendedStateManagerFeature
{
protected:
	static FWidgetStateTemporaryCache& GetStateManagerCache(UUserWidget& UserWidget);
};

/**
 * 用来存储WidgetState计算过程的一些运行时数据
 * 常用debug指令: Log LogWidgetStateManager Verbose
 */
USTRUCT()
struct VBAOVIEWMODEL_API FWidgetStateManager
{
	GENERATED_BODY()
	
	// State属于Vertex，此处规定，必须先定义才能使用
	UPROPERTY()
	TArray<FWidgetStateDefine> States;

	// 由于InstancedStruct在Editor配置时十分不方便，配置时用FName，Runtime再转换成StateDefine
	UPROPERTY()
	TMap<FWidgetStateDefine, FInstancedStructContainer> StateActionMap;
	
	// 由于InstancedStruct在Editor配置时十分不方便，配置时用FName的wrapper，Runtime再转换成FWidgetStatePair
	// 兼容预留为array，暂时只允许一个
	UPROPERTY()
	TMap<FWidgetStatePair, FActionGoingArrayWrapper> TransitionActionMap;

private:
	bool bInitialized = false;
	
	FWidgetStateDefine CurrentWidgetState;
	
	bool bInInterruptPrevious = false;
	TOptional<FWidgetStateDefine> TransitionTargetState;
	
	UPROPERTY(Transient)
	TArray<TObjectPtr<UWidgetAnimation>> PlayingTransitionAnimations;
	
	// FWidgetStateManager本身没有用到它的地方，只是作为Action的一些拓展操作的数据存储
	UPROPERTY(Transient)
	FWidgetStateTemporaryCache Cache;
	
public:
	static const FWidgetStateManager* GetStateManagerFromWidget(UUserWidget* UserWidget);
	
	// 初始化相关函数
	void Initialize(const UUserWidget* UserWidget) noexcept;
	void Initialize(UUserWidget* UserWidget, FWidgetStateDefine const& InitState) noexcept;
	void Clear() noexcept;

#if WITH_EDITOR
	void ValidateUserWidgetConfigs(UUserWidget const* UserWidget, class IWidgetCompilerLog& CompileLog) const;
#endif

	// GUIUserWidget调用入口
	void ChangeWidgetState(UUserWidget* UserWidget, FWidgetStateDefine const& NewState, bool bNeedPlayAnimation=true);
	void TransitionAnimationFinishedCallback(UUserWidget* UserWidget, const UWidgetAnimation* Animation);

	// getter
	FORCEINLINE bool IsEnabled() const noexcept
	{
		return bInitialized;
	}

	// 当没有在播过渡动画时，返回CurrentWidgetState
	// 当在播过渡动画时，返回目标WidgetState
	FORCEINLINE FWidgetStateDefine GetCurrentDesiringWidgetState() const noexcept
	{
		return TransitionTargetState.IsSet() ? TransitionTargetState.GetValue() : CurrentWidgetState;
	}

	FORCEINLINE bool IsInterrupting() const noexcept
	{
		return bInInterruptPrevious;
	}

private:
	friend struct FExtendedStateManagerFeature;
	static FWidgetStateTemporaryCache& GetStateManagerCacheFromWidget(UUserWidget& UserWidget);
	
	// 初始化、动画已播放完等情形下，无需检查过渡动画等内容，直接跳转，避免递归调用
	void ChangeStateNoTransition(UUserWidget* UserWidget, FWidgetStateDefine const& NewState);
	
	void ExecuteStateRelatedAction(UUserWidget* UserWidget, FWidgetStateDefine const& FromState, FWidgetStateDefine const& ToState);
	

	/**
	 * @return 是否播放了动效
	 */
	bool ExecuteTransitionAction(UUserWidget* UserWidget, FWidgetStateDefine const& FromState, FWidgetStateDefine const& ToState);
	
	void InterruptLastTransition(UUserWidget* UserWidget);

	// Helper functions
	bool DoesStateHasAction(FWidgetStateDefine const& InState) const;
	bool DoesTransitionHasAction(FWidgetStateDefine const& From, FWidgetStateDefine const& To) const;
	
	void ResetTransitionCache();
};