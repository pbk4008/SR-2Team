#pragma once
#ifndef __MAINAPP_H__
#define __MAINAPP_H__
#include "Base.h"
BEGIN(Engine)
class CGraphicDev;
END
class CMainApp final : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp();
public:
	HRESULT Init_MainApp();
	_int Update_MainApp(const float& fDeltaTime);
	void LateUpdate_MainApp();
	void Render_MainApp();
public:
	static CMainApp* Create();
private:
	virtual void Free();
private:
	CGraphicDev* m_pGraphicDev;
	LPDIRECT3DDEVICE9 m_pDevice;
};
#endif