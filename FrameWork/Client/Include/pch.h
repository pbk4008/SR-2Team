﻿// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"
#include "Engine_Include.h"
#include "Export_Function.h"
#include <process.h>
#include <iostream>
#include "Include.h"

#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console")
using namespace std;

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DBG_NEW
#endif

#endif

#pragma comment(lib,"System.lib")
#pragma comment(lib,"Utility.lib")


///////////////////////////////////////////////////////////////////////////
// 사운드
#include <io.h>
#include "fmod.h"
#pragma comment (lib, "fmodex_vc.lib")
///////////////////////////////////////////////////////////////////////////
// 동영상
#include <Vfw.h>
#pragma comment (lib, "vfw32.lib")
///////////////////////////////////////////////////////////////////////////

#include "SoundMgr.h"


#endif //PCH_H
