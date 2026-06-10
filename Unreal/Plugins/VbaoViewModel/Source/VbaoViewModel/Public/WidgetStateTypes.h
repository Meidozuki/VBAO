// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "UObject/Object.h"
#include "WidgetStateTypes.generated.h"

/**
 * 用来存储UMG状态的数据结构
 */
USTRUCT(BlueprintType)
struct VBAOVIEWMODEL_API FWidgetStateDefine final
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName StateID;

	// 显式声明可移动性, 使依赖可移动性的优化允许warning
	FWidgetStateDefine() noexcept = default;
	FWidgetStateDefine(const FWidgetStateDefine& Rhs) noexcept = default;
	FWidgetStateDefine(FWidgetStateDefine&& Rhs) noexcept = default;
	FWidgetStateDefine& operator=(const FWidgetStateDefine& Rhs) noexcept = default;
	FWidgetStateDefine& operator=(FWidgetStateDefine&& Rhs) noexcept = default;

	FORCEINLINE explicit FWidgetStateDefine(FName const& Name) : StateID(Name)
	{
	}

	FORCEINLINE FString ToString() const { return StateID.ToString(); }
	FORCEINLINE FName ToFName() const { return StateID; }

	FORCEINLINE bool operator==(FWidgetStateDefine const& Other) const
	{
		return StateID == Other.StateID;
	}

	FORCEINLINE bool operator!=(FWidgetStateDefine const& Other) const
	{
		return !(*this == Other);
	}

	FORCEINLINE bool operator==(FName const& InName) const
	{
		return StateID == InName;
	}

	friend FORCEINLINE uint32 GetTypeHash(FWidgetStateDefine const& Def)
	{
		return GetTypeHash(Def.StateID);
	}
};


// TPair不是UProperty，用USTRUCT包一层
USTRUCT()
struct VBAOVIEWMODEL_API FWidgetStatePair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FWidgetStateDefine FromState;

	UPROPERTY(EditAnywhere)
	FWidgetStateDefine ToState;

	FWidgetStatePair() = default;

	FWidgetStatePair(FWidgetStateDefine const& InFromState, FWidgetStateDefine const& InToState)
		: FromState(InFromState), ToState(InToState)
	{
	}

	FWidgetStatePair(struct FNamePairWrapper const& NamePair);

	FORCEINLINE bool operator==(FWidgetStatePair const& Other) const
	{
		return FromState == Other.FromState && ToState == Other.ToState;
	}

	friend FORCEINLINE uint32 GetTypeHash(FWidgetStatePair const& Pair)
	{
		return HashCombine(GetTypeHash(Pair.FromState.StateID), GetTypeHash(Pair.ToState.StateID));
	}
};


/** 
 * 状态转移完成后操作
 */
USTRUCT()
struct VBAOVIEWMODEL_API FWidgetStateActionTransitionDone
{
	GENERATED_BODY()

	virtual void DoWhenGotoState(UUserWidget* Widget, FWidgetStateDefine const& FromState,
	                             FWidgetStateDefine const& ToState)
	{
	}

	virtual ~FWidgetStateActionTransitionDone() = default;
};

/** 
 * 状态过渡操作
 */
USTRUCT()
struct VBAOVIEWMODEL_API FWidgetStateActionTransitionGoing
{
	GENERATED_BODY()

	virtual void DoBeforeChangeState(UUserWidget* Widget, FWidgetStateDefine const& FromState,
	                                 FWidgetStateDefine const& ToState)
	{
	}

	virtual ~FWidgetStateActionTransitionGoing() = default;
};

#pragma region wrappers

/**
 * 只在编辑器下用的TPair<FName>, 运行时转换成FWidgetStatePair
 */
USTRUCT()
struct FNamePairWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName FromState;

	UPROPERTY(EditAnywhere)
	FName ToState;

	FORCEINLINE bool operator==(FNamePairWrapper const& Other) const
	{
		return FromState == Other.FromState && ToState == Other.ToState;
	}

	friend FORCEINLINE uint32 GetTypeHash(FNamePairWrapper const& Pair)
	{
		return HashCombine(GetTypeHash(Pair.FromState), GetTypeHash(Pair.ToState));
	}
};

USTRUCT()
struct FNameArrayWrapper
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FName> Names;
};

// TArray<FInstancedStruct>才有Editor的编辑界面支持, 但它无法作为Map的Value，包一层USTRUCT
USTRUCT()
struct FActionDoneArrayWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,
		meta=(BaseStruct="/Script/VbaoViewModel.WidgetStateActionTransitionDone", ExcludeBaseStruct))
	TArray<FInstancedStruct> Actions;
};

USTRUCT()
struct FActionGoingArrayWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,
		meta=(BaseStruct="/Script/VbaoViewModel.WidgetStateActionTransitionGoing", ExcludeBaseStruct))
	TArray<FInstancedStruct> Actions;
};


#pragma endregion


/**
 * DataAsset只存配置数据，Runtime数据在WidgetStateManager统一计算
 */
UCLASS()
class VBAOVIEWMODEL_API UWidgetStateDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// State属于Vertex，此处规定，必须先定义才能使用
	UPROPERTY(EditDefaultsOnly, Category="UMG State", DisplayName="所有允许的StateID")
	TArray<FName> AllowedStates;

	// 定义的Vertex上的操作
	// InstanceStruct的操作太麻烦了，Editor下用FName，init的时候再转换
	UPROPERTY(EditDefaultsOnly, Category="UMG State", DisplayName="进入新状态的Action")
	TMap<FName, FActionDoneArrayWrapper> DesignTimeStateActionMap;

	// 状态过渡属于Edge上的操作，目前允许状态间任意切换，无需额外定义边
	UPROPERTY(EditDefaultsOnly, Category="UMG State", DisplayName="状态过渡时Action")
	TMap<FNamePairWrapper, FActionGoingArrayWrapper> StateTransitionActionMap;
};
