// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetStateTypes.h"



// 一些本地检查用的性能check
#if WITH_EDITOR

static_assert(std::is_trivially_copyable_v<FWidgetStateDefine>);
static_assert(std::is_trivially_move_constructible_v<FWidgetStateDefine>);
static_assert(std::is_trivially_move_assignable_v<FWidgetStateDefine>);

static_assert(std::is_trivially_copyable_v<FWidgetStatePair>);
static_assert(std::is_trivially_move_constructible_v<FWidgetStatePair>);
static_assert(std::is_trivially_move_assignable_v<FWidgetStatePair>);

#endif

FWidgetStatePair::FWidgetStatePair(struct FNamePairWrapper const& NamePair)
	:FromState(NamePair.FromState), ToState(NamePair.ToState)
{}
