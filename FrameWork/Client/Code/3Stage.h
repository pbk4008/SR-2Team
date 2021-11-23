#pragma once
#ifndef __3STAGE_H__
#define __3STAGE_H__
#include "Scene.h"
class CLoading;
class CDoor;
class CPlayer;
class C3Stage final : public CScene
{
private:
	explicit C3Stage();
	explicit C3Stage(LPDIRECT3DDEVICE9 pDevice);
	virtual ~C3Stage();
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
	HRESULT Init_Loading_Layer();
	
public:
	static C3Stage* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
private:
	CLoading* m_pLoading;
	CPlayer* m_pPlayer;
};
#endif