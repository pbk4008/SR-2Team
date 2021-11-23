#pragma once
#ifndef __FlyMon_h__
#define __FlyMon_h__

#include "Monster.h"
#include "Player.h"
BEGIN(Engine)
class CSphereCollision;
END
class CFlyMon : public CMonster
{
public:
	enum class STATE {IDLE, WALKING, ATTACK, DEATH, MAX};
private:
	explicit CFlyMon();
	explicit CFlyMon(LPDIRECT3DDEVICE9 pDevice);
	explicit CFlyMon(const CFlyMon& rhs);
	virtual ~CFlyMon();

public:
	HRESULT Init_FlyMon();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject() override;
	virtual CGameObject* Clone_GameObject() override;
	virtual void ResetObject();
	HRESULT SettingAnimator();
	CCollision* getCollider() { return m_pCollision; }

private:
	void Change_State();
	void HitMonster(const _float& fTimeDelta);

public:
	static CFlyMon* Create(LPDIRECT3DDEVICE9 pDevice);

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

	STATE			m_eCurState;
	STATE			m_ePreState;

	CSphereCollision* m_pCollision; // 몬스터가 맞는 충돌
	CSphereCollision* m_pAttackColl; // 몬스터가 플레이어 공격하는 충돌

	_int			m_iHP;

	_bool			m_bAttack;
	_bool			m_bMoving;

	_bool			m_bTracking;

	_float			m_iTimer;
	_float			m_fSpeed;
	_float			m_fAttackDelay;

};

#endif // FlyMon_h__