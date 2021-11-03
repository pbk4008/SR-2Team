#pragma once
#ifndef __LOGO_H__
#define __LOGO_H__
#include "Scene.h"
class CLoading;
class CBackGround;
class CLogo final : public CScene
{
private:
	explicit CLogo();
	explicit CLogo(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLogo();
public:
	virtual HRESULT Init_Scene() override;
	virtual _int Update_Scene(const _float& fDeltaTime) override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
private:
	HRESULT Init_Layer();
	HRESULT Init_Environment_Layer();
	HRESULT Init_GameLogic_Layer();
	HRESULT Init_UI_Layer();
	HRESULT Init_LogoScene();
public:
	static CLogo* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
private:
	CBackGround* m_pBackGround;
	CLoading* m_pLoading;
};
#endif