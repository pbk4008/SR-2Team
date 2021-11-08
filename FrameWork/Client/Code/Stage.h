#pragma once
#ifndef __STAGE_H__
#define __STAGE_H__
#include "Scene.h"
class CLoading;
class CStage final : public CScene
{
private:
	explicit CStage();
	explicit CStage(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CStage();
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
public:
	static CStage* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
private:
	CLoading* m_pLoading;
};
#endif