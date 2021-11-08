#pragma once
#ifndef __ShootMon_h__
#define __ShootMon_h__

#include "Monster.h"
#include "Player.h"

class CShootMon : public CMonster
{
private:
	explicit CShootMon();
	explicit CShootMon(LPDIRECT3DDEVICE9 pDevice);
	explicit CShootMon(const CShootMon& rhs);
	virtual ~CShootMon();

public:
	HRESULT Init_ShootMon();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual CGameObject* Clone_GameObject() override;

public:
	static CShootMon* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual HRESULT Add_Component();
	virtual void Follow(const _float& fDeltaTime);
	virtual void Attack(const _float& fDeltaTime);
	virtual void Free();

private:
	CRcTex*			m_pBufferCom;
	CTexture*		m_pTexture;
	
	_bool			m_bAttack;
	_bool			m_bMoving;

	_float			m_iTimer;
	_float			m_fSpeed;
};

#endif // ShootMon_h__