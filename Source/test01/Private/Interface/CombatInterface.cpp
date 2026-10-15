// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/CombatInterface.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.
// 默认实现：返回原点（0,0,0）。
// 未覆写该接口的具体类会直接使用此处默认值，通常应覆写此函数返回真实挂点位置。
FVector ICombatInterface::GetCombatSocketLocation()
{
	return FVector();
}
