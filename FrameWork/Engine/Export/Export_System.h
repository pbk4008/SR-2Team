#ifndef __EXPORT_SYSTEM_H__
#define __EXPORT_SYSTEM_H__
#include "Engine_Include.h"
#include "GraphicDev.h"
#include "TimeMgr.h"
#include "FrameMgr.h"
#include "InputDev.h"

BEGIN(Engine)
//GraphicDev
inline HRESULT Init_GraphicDev(WINDOW eMode, const _long& dwCntX, const _long& dwCntY, HWND hWnd, CGraphicDev** pDevice);
inline void Render_Begin(D3DXCOLOR color);
inline void Render_End();

//TimeMgr
inline HRESULT Init_TimeMgr();
inline void Update_TimeMgr();
inline _float GetDeltaTime();
//FrameMgr
inline HRESULT Init_FrameMgr(const _float& fLimitFrame);
inline _bool IsPermit(const _float& fDeltaTime);
//InputDev
inline void Update_InputDev();
inline _bool Key_Up(_ulong dwKey);
inline _bool Key_Down(_ulong dwKey);
inline _bool Key_Pressing(_ulong dwKey);
inline _vec3 MousePos(HWND _hWnd);

inline void System_Release();
#include "Export_System.inl"
END

#endif