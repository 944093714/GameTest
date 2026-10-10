// Fill out your copyright notice in the Description page of Project Settings.

#include "MyInterface.h"

// UE 的 UINTERFACE 反射机制会在链接期引用接口虚函数的地址(配合 Cast<IMyInterface> / TScriptInterface 使用),
// 所以即使这些函数声明为纯虚/被实现类 override,接口也必须在此提供定义,否则链接报 LNK2001。
// 实际的高亮逻辑在实现类 AMyEnemy 里 override,见 Character/MyEnemy.cpp。
void IMyInterface::HighlightActor()
{
}

void IMyInterface::UnHighlightActor()
{
}