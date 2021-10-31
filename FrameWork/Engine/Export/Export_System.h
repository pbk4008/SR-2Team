#ifndef __EXPORT_SYSTEM_H__
#define __EXPORT_SYSTEM_H__
#include "Engine_Include.h"
#include "GraphicDev.h"

BEGIN(Engine)
inline HRESULT Init_GraphicDev(WINDOW eMode, const _long& dwCntX, const _long& dwCntY, HWND hWnd, CGraphicDev** pDevice);
inline void Render_Begin(D3DXCOLOR color);
inline void Render_End();

inline void System_Release();
#include "Export_System.inl"
END

#endif