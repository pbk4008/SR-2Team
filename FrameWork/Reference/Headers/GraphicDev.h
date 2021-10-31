#pragma once
#ifndef __GRAPICDEV_H__
#define __GRAPICDEV_H__
#include "Base.h"
BEGIN(Engine)
class ENGINE_DLL CGraphicDev final : public CBase
{
DECLARE_SINGLETON(CGraphicDev)
private:
	explicit CGraphicDev();
	virtual ~CGraphicDev();
public:
	HRESULT Init_GraphicDev(WINDOW eMode, const _long& dwCntX, const _long& dwCntY, HWND hWnd, CGraphicDev** pDevice);
	void Render_Begin(D3DXCOLOR color);
	void Render_End();
private:
	void SetParameter(D3DPRESENT_PARAMETERS& d3pp, WINDOW eMode, const _long& dwCntX, const _long& dwCntY, HWND hWnd);
private:
	virtual void Free();
public:
	inline LPDIRECT3DDEVICE9 getDevice() { return m_pDevice; }
private:
	LPDIRECT3D9 m_pSDK;
	LPDIRECT3DDEVICE9 m_pDevice;
};
END
#endif