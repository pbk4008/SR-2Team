#pragma once
#ifndef __STAGE_H__
#define __STAGE_H__
#include "Scene.h"
class CLoading;
class CPlayer;
class CSpawner;
class CDoorObserver;
class CDoor;
class C1Stage final : public CScene
{
private:
	explicit C1Stage();
	explicit C1Stage(LPDIRECT3DDEVICE9 pDevice);
	virtual ~C1Stage();
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
	void DoorSetting();
	void setClearBox();
	void FloorClear();
public:
	static C1Stage* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
private:
	CLoading* m_pLoading;
	vector<CDoorObserver*> m_vecDoorObserver;
	vector<CDoor*> m_vecDoor;
	vector<CGameObject*> m_vecClearBox;
	CSpawner* m_pSpawner;

	_bool m_bFloorClear;
	_bool m_bFirst;
	CPlayer* m_pPlayer;
};
#endif