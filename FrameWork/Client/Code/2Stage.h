#pragma once
#ifndef __2STAGE_H__
#define __2STAGE_H__
#include "Scene.h"
class CLoading;
class CDoor;
class CPlayer;
class CPotal;
class C2Stage final : public CScene
{
private:
	explicit C2Stage();
	explicit C2Stage(LPDIRECT3DDEVICE9 pDevice);
	virtual ~C2Stage();
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
	static C2Stage* Create(LPDIRECT3DDEVICE9 pDevice);
private: void Init_Fog(_ulong Color, _ulong Mode, BOOL UseRange, _float Density);
private:
	virtual void Free();
private:
	CLoading* m_pLoading;
	CPlayer* m_pPlayer;
	CPotal* m_pPotal;

};
#endif