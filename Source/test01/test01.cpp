// Copyright Epic Games, Inc. All Rights Reserved.

#include "test01.h"
#include "Modules/ModuleManager.h"

// 注册 test01 为游戏的主模块（单游戏项目只有一个主模块，负责游戏启动/关停）。
// 使用默认的 FDefaultGameModuleImpl（本模块无需自定义 StartupModule 逻辑）。
IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, test01, "test01" );
