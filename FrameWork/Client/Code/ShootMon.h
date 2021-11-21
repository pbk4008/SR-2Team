#pragma once
#ifndef __ShootMon_h__
#define __ShootMon_h__

#include "Monster.h"
#include "Player.h"
#include "MonBullet.h"

class CBullet;
class CShootMon : public CMonster
{
public:
	enum class STATE {IDLE, WALKING, ATTACK, DEATH, MAX};
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
	HRESULT SettingAnimator();

	CCollision* getCollider() { return m_pCollision; }

private:
	void Change_State();
	CBullet* Shoot(GAMEOBJECTID eID);


public:
	static CShootMon* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual HRESULT Add_Component();
	virtual void Follow(const _float& fDeltaTime);
	virtual void Attack(const _float& fDeltaTime);
	virtual void Attack_Dis(const _float& fDeltaTime);
	virtual void Free();

private:
	CRcTex*			m_pBufferCom;
	CTexture*		m_pTexture;
	CAnimator*		m_pAnimator;
	CMonBullet*		m_pMonBullet;

	STATE			m_eCurState;
	STATE			m_ePreState;

	CCollision* m_pCollision; // ���Ͱ� �´� �浹
	CCollision* m_pAttackColl; // ���Ͱ� �÷��̾� �����ϴ� �浹

	_int			m_iHP;

	_bool			m_bAttack;
	_bool			m_bMoving;

	_float			m_iTimer;
	_float			m_fSpeed;
};

#endif // ShootMon_h__