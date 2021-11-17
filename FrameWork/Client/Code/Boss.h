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
	virtual HRESULT Add_Component();
	virtual void	 Follow(const _float& fDeltaTime);
	virtual void	 Attack_Dis(const _float& fDeltaTime);
	virtual void	 AttackHit(const _float& fDeltaTime);
	virtual void	 MeleeAttack(const _float& fDeltaTime);
	virtual void	 RangeAttack(const _float& fDeltaTime);
	virtual void	 ChargeAttack(const _float& fDeltaTime);
	virtual void	 TeleportPillerAttack(const _float& fTimeDelta);
	virtual void	 HPCheck();
	virtual void	 Attack_Type(const _float& fDeltaTime);
	virtual void	 Chase(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	virtual void	 ChaseRange(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	virtual _matrix* ComputeLookAtTarget(const _vec3* pTargetPos);
	CBullet* Fireball(GAMEOBJECTID eID);

	virtual void	 Free();

public:
	static CBoss* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	CRcTex*			m_pBufferCom;
	CTexture*		m_pTexture;
	CAnimator*		m_pAnimator;

	STATE			m_eCurState;
	STATE			m_ePreState;

	CCollision*		m_pCollision; // 몬스터가 맞는 충돌
	CCollision*		m_pAttackColl; // 몬스터가 플레이어 공격하는 충돌

	_int			m_iHP;
	_int			m_iAttackNumber;

	_bool			m_bAttack;
	_bool			m_bMoving;
	_bool			m_bChargeAttack;

	_float			m_iTimer;
	_float			m_fSpeed;
};
//    : m_pBufferCom(nullptr), m_pTexture(nullptr), m_pAnimator(nullptr), 
//		m_eCurState, m_ePreState, m_pCollision(nullptr), m_pAttackColl(nullptr), 
//		m_iHP, m_bAttack, m_bMoving, m_iTimer, m_fSpeed


#endif // Boss_h__