// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetStateManager.h"

#include "StatefulUMGInterface.h"
#include "WidgetStateActions.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Editor/WidgetCompilerLog.h"

DEFINE_LOG_CATEGORY(LogWidgetStateManager)

UWidgetAnimation* GetAnimationByName(UUserWidget* Widget, const FString& InAnimKey)
{
	if (UWidgetBlueprintGeneratedClass* WidgetClass = Widget->GetWidgetTreeOwningClass())
	{
		const FString& ComparisonAnimKey = 
			InAnimKey.Contains(TEXT("_INST")) ? InAnimKey : FString::Printf(TEXT("%s_INST"), *InAnimKey);
		for (int32 i = 0; i < WidgetClass->Animations.Num(); ++i)
		{
			if (UWidgetAnimation* AnimInst = WidgetClass->Animations[i])
			{
				const FString AnimInstName = AnimInst->GetName();
				if (AnimInstName.Equals(ComparisonAnimKey, ESearchCase::IgnoreCase))
				{
					return AnimInst;
				}
			}
		}
	}
	return nullptr;
}


FWidgetStateTemporaryCache& FExtendedStateManagerFeature::GetStateManagerCache(UUserWidget& UserWidget)
{
	return FWidgetStateManager::GetStateManagerCacheFromWidget(UserWidget);
}

FWidgetStateTemporaryCache& FWidgetStateManager::GetStateManagerCacheFromWidget(UUserWidget& UserWidget)
{
	IStatefulUMG* StatefulUMG = Cast<IStatefulUMG>(&UserWidget);
	check(StatefulUMG);
	return StatefulUMG->GetWidgetStateManager().Cache;
}

const FWidgetStateManager* FWidgetStateManager::GetStateManagerFromWidget(UUserWidget* UserWidget)
{
	if (UserWidget)
	{
		if (IStatefulUMG* StatefulUMG = Cast<IStatefulUMG>(UserWidget))
		{
			return &StatefulUMG->GetWidgetStateManager();
		}
	}
	return nullptr;
}

void FWidgetStateManager::Initialize(const UUserWidget* UserWidget) noexcept
{
	if (!IsValid(UserWidget))
	{
		return;
	}
	if (bInitialized)
	{
		UE_LOG(LogWidgetStateManager, Warning, TEXT("FWidgetStateManager::Initialize duplicately called!") );
		return;
	}

	const IStatefulUMG* StatefulUMG = Cast<IStatefulUMG>(UserWidget);
	if (!StatefulUMG)
	{
		return;
	}
	const UWidgetStateDataAsset* StateConfig = StatefulUMG->GetWidgetStateDataAsset();
	if (StateConfig == nullptr)
	{
		return;
	}

	// 转换States
	States.Append(StateConfig->AllowedStates);
	
	for (FName const& State: StateConfig->AllowedStates)
	{
		if (!States.Contains(FWidgetStateDefine(State)))
		{
			continue;
		}
		
		// 将State上的操作从 配置友好 的形式转换成Runtime数据
		// FName -> StateDefine, TArray<InstancedStruct> -> InstancedStructContainer
		if (const FActionDoneArrayWrapper* Wrapper = StateConfig->DesignTimeStateActionMap.Find(State))
		{
			// 这一步构建Container操作可能会比较耗，但是InstancedStructContainer内存连续，运行性能理论上会更好一些
			TConstArrayView<FInstancedStruct> View(Wrapper->Actions);
			FInstancedStructContainer ISArray;
			ISArray.Append(View);
			StateActionMap.Emplace(State, MoveTemp(ISArray));
		}
	}

	// 将Transition操作从FName Pair改成FWidgetStatePair
	for (const auto& Pair: StateConfig->StateTransitionActionMap)
	{
		const FWidgetStatePair& StatePair(Pair.Key);
		if (!States.Contains(StatePair.FromState) || !States.Contains(StatePair.ToState))
		{
			continue;
		}
		TransitionActionMap.Emplace(StatePair, Pair.Value);
	}
	
#if WITH_EDITOR
	// ensure防止开发期漏逻辑
	ensure(States.Num() == StateConfig->AllowedStates.Num());
	ensure(StateActionMap.Num() == StateConfig->DesignTimeStateActionMap.Num());
	ensure(TransitionActionMap.Num() == StateConfig->StateTransitionActionMap.Num());
#endif
	
	bInitialized = true;
	UE_LOG(LogWidgetStateManager, Log, TEXT("WidgetStateManager initialized for %s"), *UserWidget->GetName());
}

void FWidgetStateManager::Initialize(UUserWidget* UserWidget, FWidgetStateDefine const& InitState) noexcept
{
	if (!bInitialized)
	{
		Initialize(UserWidget);
	}

	// 初始化State. 初始化不用播放动画，直接设置状态
	ChangeStateNoTransition(UserWidget, InitState);
}

void FWidgetStateManager::Clear() noexcept
{
	States.Empty();
	StateActionMap.Empty();
	TransitionActionMap.Empty();
	ResetTransitionCache();
}

#if WITH_EDITOR
void FWidgetStateManager::ValidateUserWidgetConfigs(UUserWidget const* UserWidget,
                                                    IWidgetCompilerLog& CompileLog) const
{
	if (UserWidget == nullptr)
	{
		return;
	}

	const IStatefulUMG* StatefulUMG = Cast<IStatefulUMG>(UserWidget);
	if (!StatefulUMG)
	{
		return;
	}
	const UWidgetStateDataAsset* StateConfig = StatefulUMG->GetWidgetStateDataAsset();
	if (StateConfig == nullptr)
	{
		return;
	}
	
	TArray<FName> const& AllStates = StateConfig->AllowedStates;
	
	FString DumpString;
	for (FName const& State: AllStates)
	{
		DumpString += State.ToString() + TEXT(", ");
	}
	
	// 检查Vertex操作是否都对应了Vertex
	for (auto& [Name, _]: StateConfig->DesignTimeStateActionMap)
	{
		if (!AllStates.Contains(Name))
		{
			CompileLog.Warning(FText::FromString(FString::Printf(
				TEXT("[StateActionMap]Cannot find state *%s* in: %s"),
				*Name.ToString(), *DumpString
				)));
		}
	}
	
	// 检查Edge操作是否都对应了Vertex
	for (auto& [NamePair, _]: StateConfig->StateTransitionActionMap)
	{
		auto& [From, To] = NamePair;
		FString PairString = FString::Printf(TEXT("<%s, %s>"), *From.ToString(), *To.ToString());
		if (!AllStates.Contains(From))
		{
			CompileLog.Warning(FText::FromString(FString::Printf(
				TEXT("[TransitionActionMap]Cannot find %s's From state *%s* in: %s"),
				*PairString, *From.ToString(), *DumpString
				)));
		}
		if (!AllStates.Contains(To))
		{
			CompileLog.Warning(FText::FromString(FString::Printf(
				TEXT("[TransitionActionMap]Cannot find %s's To state *%s* in: %s"),
				*PairString, *From.ToString(), *DumpString
				)));
		}
	}
}
#endif

bool FWidgetStateManager::DoesStateHasAction(FWidgetStateDefine const& InState) const
{
	return StateActionMap.Contains(InState);
}

bool FWidgetStateManager::DoesTransitionHasAction(FWidgetStateDefine const& From, FWidgetStateDefine const& To) const
{
	return TransitionActionMap.Contains(FWidgetStatePair(From, To));
}

void FWidgetStateManager::ResetTransitionCache()
{
	TransitionTargetState.Reset();
	PlayingTransitionAnimations.Empty();
}


void FWidgetStateManager::InterruptLastTransition(UUserWidget* UserWidget)
{
	if (UserWidget == nullptr)
	{
		return;
	}
	
	if (TransitionTargetState.IsSet())
	{
		// 只有唯一入口调用，Widget信息在ChangeState打印过了，这里不重复打了
		UE_LOG(LogWidgetStateManager, Verbose, TEXT("Interrupting previous anim"));
		bInInterruptPrevious = true;
		// 此时上一个过渡动画还没播完
		// 例如A->B的动画还没播完，开始B->C
		// 需要 1)停止AB的动画，然后 2)切到B
		// 但是Stop的callback可能到下一帧才调用，所以这里 3)直接做Cache清理，防止被影响
		// 在下一帧Callback，AB动画结束时，因为WidgetAnimation*的判断，所以不会在BC过程中被拉回B
		for (UWidgetAnimation* Anim: PlayingTransitionAnimations)
		{
			UserWidget->StopAnimation(Anim);
		}
		FWidgetStateDefine NextState = TransitionTargetState.GetValue();
		ResetTransitionCache();
		ChangeStateNoTransition(UserWidget, NextState);
		bInInterruptPrevious = false;
	}
}

void FWidgetStateManager::ChangeWidgetState(UUserWidget* UserWidget, FWidgetStateDefine const& NewState, bool bNeedPlayAnimation)
{
	if (!IsValid(UserWidget) || !bInitialized)
	{
		return;
	}
	
	// 边界情况之，2个状态快速相互切换时，上一个动画还没完成，需要从TargetState中取出
	// 边界情况之，切换A状态并过渡中时，再次请求A状态，此时不能强行让动画结束，而只能从状态中取
	FWidgetStateDefine DesiringState = GetCurrentDesiringWidgetState();

	FString Context;
#if WITH_EDITOR
	Context = FString::Printf(TEXT("Client %d "), UE::GetPlayInEditorID());
#endif
	UE_LOG(LogWidgetStateManager, VeryVerbose, TEXT("%sWidget (%s) request ChangeWidgetState %s -> %s"),
		*Context, *GetNameSafe(UserWidget), *DesiringState.ToString(), *NewState.ToString()
		);
	
	// 有一种极特殊边界情况，播放到Vanilla状态的动效，此时UI被隐藏，动效被暂停了，会在下次Show的时候继续播放
	// 因此引入了不播动效的bool
	// 如果为true，走正常播放动效，因此Desire和New一致时，等待动效完成
	// 如果为false，此时其实就算Desire和New一致，期望也是能直接跳到New，而不是等待动效完成，因此此时需要走到下方打断动画
	if (bNeedPlayAnimation && DesiringState == NewState)
	{
		UE_LOG(LogWidgetStateManager, VeryVerbose, TEXT("Current state is same as New state. return"));
		return;
	}
	
	// 清理上一个过渡状态
	InterruptLastTransition(UserWidget);
	// Interrupt之后，Desiring和Current一致
	ensureAlways(DesiringState == CurrentWidgetState);
	
	if (CurrentWidgetState == NewState)
	{
		// 如果不播动效切Desire==New，此时顶点逻辑已经在Interrupt中执行，不需要再走下方逻辑
		return;
	}

	// 尝试播放状态切换动画
	bool bIsPlayingAnimation = false;
	if (bNeedPlayAnimation)
	{
		if (DoesTransitionHasAction(CurrentWidgetState, NewState))
		{
			bIsPlayingAnimation = ExecuteTransitionAction(UserWidget, CurrentWidgetState, NewState);
		}
	}
	
	// 没有播放动画，直接切WidgetStyle
	if (!bIsPlayingAnimation)
	{
		ChangeStateNoTransition(UserWidget, NewState);
	}
}

void FWidgetStateManager::ChangeStateNoTransition(UUserWidget* UserWidget, FWidgetStateDefine const& NewState)
{
	if (UserWidget == nullptr)
	{
		return;
	}
	
	if (!States.Contains(NewState))
	{
		UE_LOG(LogWidgetStateManager, Warning, TEXT("Attempt to change to unregistered widget state:%s (Widget %s)!"),
			   *NewState.ToString(), *UserWidget->GetName());
		
		FString DumpString;
		for (FWidgetStateDefine const& State: States)
		{
			DumpString += State.ToString() + TEXT(", ");
		}
		UE_LOG(LogWidgetStateManager, Verbose, TEXT("State %s not found in states: %s"),
			*NewState.ToString(), *DumpString);
		return;
	}
	
	
	if (!DoesStateHasAction(NewState))
	{
		CurrentWidgetState = NewState;
	}
	else
	{
		const FWidgetStateDefine PrevState = CurrentWidgetState;
		CurrentWidgetState = NewState;
		
		// 设置WidgetStyle之类的操作
		ExecuteStateRelatedAction(UserWidget, PrevState, NewState);
	}
	
	UE_LOG(LogWidgetStateManager, Verbose, TEXT("Widget (%s) succeed to change to state:%s"),
		*UserWidget->GetName(), *NewState.ToString());
	
}

void FWidgetStateManager::ExecuteStateRelatedAction(UUserWidget* UserWidget,
                                                    FWidgetStateDefine const& FromState, FWidgetStateDefine const& ToState)
{
	FInstancedStructContainer* ArrayPtr = StateActionMap.Find(ToState);
	if (ArrayPtr == nullptr)
	{
		return;
	}

	for (FStructView View: *ArrayPtr)
	{
		if (FWidgetStateActionTransitionDone * Action = View.GetPtr<FWidgetStateActionTransitionDone>())
		{
			Action->DoWhenGotoState(UserWidget, FromState, ToState);
		}
	}
}

bool FWidgetStateManager::ExecuteTransitionAction(UUserWidget* UserWidget, FWidgetStateDefine const& FromState, FWidgetStateDefine const& ToState)
{
	if (UserWidget == nullptr)
	{
		return false;
	}
	
	if (FromState == ToState)
	{
		return false;
	}
	// 虽然兼容了其他操作，但是目前看来绝大部分过渡操作都是播动效

	// 找到配置项
	FWidgetStatePair StatePair(FromState, ToState);
	if (TransitionActionMap.Contains(StatePair))
	{
		auto& Array = TransitionActionMap[StatePair].Actions;
		if (Array.Num() != 1)
		{
			return false;
		}

		// 兼容预留为array，暂时只允许一个
		FInstancedStruct& Ref = Array[0];
		// 取出需要播放的动画
		FDefaultActionTransitionGoing* Action = Ref.GetMutablePtr<FDefaultActionTransitionGoing>();
		if (Action == nullptr)
		{
			return false;
		}
		// 做预操作
		Action->DoBeforeChangeState(UserWidget, FromState, ToState);

		for (const FName& AnimName: Action->AnimationNames)
		{
			if (UWidgetAnimation* Animation = GetAnimationByName(UserWidget, AnimName.ToString()))
			{
				UserWidget->PlayAnimation(Animation);
				// 缓存目标状态和动画，等待播放完毕切状态
				TransitionTargetState = ToState;
				PlayingTransitionAnimations.AddUnique(Animation);
				UE_LOG(LogWidgetStateManager, Verbose, TEXT("Widget (%s) playing transition (%s->%s) animation:%s"),
					*UserWidget->GetName(), *FromState.ToString(), *ToState.ToString(), *AnimName.ToString()
					);
				
			}
			else
			{
				UE_LOG(LogWidgetStateManager, Warning, TEXT("Widget (%s) cannot find transition (%s->%s) animation:%s"),
					*UserWidget->GetName(), *FromState.ToString(), *ToState.ToString(), *AnimName.ToString()
					);
			}
			
		}
		return !PlayingTransitionAnimations.IsEmpty();
	}
	
	return false;
}

void FWidgetStateManager::TransitionAnimationFinishedCallback(UUserWidget* UserWidget, const UWidgetAnimation* Animation)
{
	if (!IsValid(UserWidget) || !IsValid(Animation))
	{
		return;
	}
	if (!bInitialized)
	{
		// 没初始化,说明没有走State这一套,无视
		return;
	}
	
	// 正常播放完之后的callback
	if (!TransitionTargetState.IsSet() || PlayingTransitionAnimations.IsEmpty())
	{
		// UE_LOG(LogWidgetStateManager, Error, TEXT("UUserWidget::TransitionAnimationFinishedCallback: TransitionTargetState %d or TransitionAnimation %d is not set!"), TransitionTargetState.IsSet(), TransitionAnimation.IsSet());
		return;
	}
	
	// 这个函数有可能是InterruptLast函数内StopAnim之后触发的，此时数组和State都交给外面处理
	if (bInInterruptPrevious)
	{
		return;
	}

	if (PlayingTransitionAnimations.Contains(Animation))
	{
		PlayingTransitionAnimations.Remove(const_cast<UWidgetAnimation*>(Animation));
		UE_LOG(LogWidgetStateManager, Verbose, TEXT("Anim %s done, remain %d anims playing. (Widget %s)"),
			*Animation->GetName(), PlayingTransitionAnimations.Num(), *UserWidget->GetName()
		);
		if (PlayingTransitionAnimations.IsEmpty())
		{
			// 播放完了Transition动画，切状态
			ChangeStateNoTransition(UserWidget, TransitionTargetState.GetValue());
			ResetTransitionCache();
		}
	}
	else
	{
		// 可能在Transition期间有其他动效同时播放，在VXE没清理完的情况下，先忽略
		// UE_LOG(LogWidgetStateManager, Display, TEXT("Another Animation is playing during Transition. Animation:%s, TransitionAnimation:%s"), *GetNameSafe(Animation), *GetNameSafe(TransitionAnimation.GetValue()));
	}
}

