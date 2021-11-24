#pragma once
#ifndef __3STAGE_H__
#define __3STAGE_H__
#include "Scene.h"
class CLoading;
class CDoor;
class CPlayer;
class CMonster;
class CPotal;
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

	void Init_Fog(_ulong Color, _ulong Mode, BOOL UseRange, _float Density);

	void LoadMeleeMon();
	void LoadShootMon();
	void LoadFlyMon();
	
public:
	static C3Stage* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
private:
	CLoading* m_pLoading;
	CPlayer* m_pPlayer;
	CPotal* m_pPotal;

	_bool m_bPotalSpawn;
	_bool m_bMeleeSpawn;
	_bool m_bShootSpawn;
	_bool m_bFlySpawn;



	_bool m_b1;
	_bool m_b2;
	_bool m_b3;
	_bool m_b4;
	_bool m_b5;
	_bool m_b6;
	_bool m_b7;
	_bool m_b8;
	_bool m_b9;
	_bool m_b10;
	_bool m_b11;
	_bool m_b12;
};
#endif