#ifndef __ENGINE_INCLUDE_H__
#define __ENGINE_INCLUDE_H__
#include <vector>
#include <array>
#include <list>
#include <map>
#include <string>
#include <chrono>
#include <unordered_map>
#include <algorithm>
#include <d3d9.h>
#include <d3dx9.h>

using namespace std;
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")

#include "Engine_Macro.h"
#include "Engine_TypeDef.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"
#include "Engine_Enum.h"
#include "Engine_Struct.h"

#pragma warning(disable : 4251)

using namespace Engine;

#endif