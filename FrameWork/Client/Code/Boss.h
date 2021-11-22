#pragma once
#ifndef __Boss_h__
#define __Boss_h__

#include "GameObject.h"
#include "Player.h"

class CBullet;
class CBoss : public CGameObject
{
public:
	enum class STATE { IDLE, WALKING, MELEE, RANGE, PILLAR, CHARGE, DEATH, MAX };

private:
	explicit CBoss();
	explicit CBoss(LPDIRECT3DDEVICE9 pDevice);
	explicit CBoss(const CBoss& rhs);
	virtual ~CBoss();

public:
	HRESULT Init_Boss();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject() override;
	virtual CGameObject* Clone_GameObject() override;
	HRESULT SettingAnimator();

private:
	void Change_State();
	void GetHit(const _float fDeltaTime);
	virtual HRESULT Add_Component();

	virtual void	 HPCheck();
	virtual void	 Follow(const _float& fDeltaTime);
	virtual void	 Attack_Dis(const _float& fDeltaTime);
	virtual void	 MeleeAttack(const _float& fDeltaTime);
	virtual void	 RangeAttack(const _float& fDeltaTime);
	virtual void	 ChargeTarget(const _float& fDeltaTime);
	virtual void	 ChargeAttack(const _float& fDeltaTime, const _vec3 vDir, const _vec3 vPos, const _vec3 vPlayerPos);

	virtual void	 HitPlayer(const _float& fDeltaTime);
	virtual void	 HitBoss(const _float& fTimeDelta);

	virtual void	 Chase(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	virtual void	 ChaseRange(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	virtual void	 ChaseCharge(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);

	virtual _matrix* ComputeLookAtTarget(const _vec3* pTargetPos);
	CBullet* Fireball(GAMEOBJECTID eID);


public:
	_int getHP() { return m_iHP; }
	void setPos(const _vec3& vPos);
	void setLook(const _vec3& vLook);
	void setTarget(const _vec3& vTarget);
	virtual void	 Free();

public:
	static CBoss* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	CRcTex*			m_pBufferCom;
	CTexture*		m_pTexture;
	CAnimator*		m_pAnimator;

	STATE			m_eCurState;
	STATE			m_ePreState;

	CSphereCollision*		m_pCollision; // 몬스터가 맞는 충돌
	CSphereCollision*		m_pAttackColl; // 몬스터가 플레이어 공격하는 충돌

	//CCollision* m_pCollision;
	//CCollision* m_pAttackColl;

	_int			m_iHP;
	_int			m_iAttackNumber;

	_bool			m_bAttack;
	_bool			m_bMoving;
	_bool			m_bChargeAttack;

	_float			m_iTimer;
	_float			m_fFireballTimer;
	_float			m_fSpeed;
	_float			m_fChargeTime;

	_vec3			m_vTargetPos;
	_vec3			m_vFirstPos;
	_vec3			m_vLook;
};

#endif // Boss_h__