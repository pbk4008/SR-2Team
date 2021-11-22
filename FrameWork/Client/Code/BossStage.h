#pragma once
#ifndef __BOSSSTAGE_H__
#define __BOSSSTAGE_H__
#include "Scene.h"
class CLoading;
class CDoor;
class CPlayer;
class CBossStage final : public CScene
{
private:
	explicit CBossStage();
	explicit CBossStage(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CBossStage();
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
	static CBossStage* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
private:
	CLoading* m_pLoading;
	CPlayer* m_pPlayer;
};

#endif