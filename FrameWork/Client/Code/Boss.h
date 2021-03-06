#pragma once
#ifndef __Boss_h__
#define __Boss_h__

#include "GameObject.h"
#include "Player.h"

class CBullet;
class CBoss : public CGameObject
{
public:
	enum class STATE { IDLE, WALKING, MELEE, RANGE, DEATH, MAX };
	enum class PHASE {PHASE1, PHASE2, MAX};

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

	virtual void	 HPCheck();
	virtual void	 Follow(const _float& fDeltaTime);
	virtual void	 Attack_Dis(const _float& fDeltaTime);
	virtual void	 MeleeAttack(const _float& fDeltaTime);
	virtual void	 RangeAttack(const _float& fDeltaTime);

	//virtual void	 HitPlayer(const _float& fDeltaTime);  // 보스가 플레이어 공격하는 충돌
	virtual void	 HitBoss(const _float& fTimeDelta);	   // 보스가 맞는 충돌

	virtual void	 Chase(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);
	virtual void	 ChaseRange(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);

	virtual _matrix* ComputeLookAtTarget();
	CBullet* Fireball(GAMEOBJECTID eID);


public:
	virtual const _int& getHP() const { return m_iHP; }
	virtual const _bool& getHPHit() const { return m_bHPHit; }
	CCollision* getCollider() { return m_pCollision; }

	
	virtual void	 Free();

public:
	static CBoss* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	CRcTex*			m_pBufferCom;
	CTexture*		m_pTexture;
	CAnimator*		m_pAnimator;

	STATE			m_eCurState;
	STATE			m_ePreState;

	PHASE			m_ePhase;

	CSphereCollision*		m_pCollision; // 몬스터가 맞는 충돌
	CSphereCollision*		m_pAttackColl; // 몬스터가 플레이어 공격하는 충돌

	_int			m_iHP;
	_int			m_iAttackNumber;

	_bool			m_bAttack;
	_bool			m_bMoving;
	_bool			m_bChargeAttack;
	_bool			m_bHPHit;

	_float			m_iTimer;
	_float			m_fFireballTimer;
	_float			m_fSpeed;
	_float			m_fChargeTime;
	_float			m_fAttackDelay;

	_vec3			m_vTargetPos;
	_vec3			m_vFirstPos;
	_vec3			m_vLook;

};

#endif // Boss_h__

//if (m_pCollision->getHit())
//{
//	m_iHP -= 50;
//	cout << "Boss got Hit!" << endl;
//	m_pCollision->ResetCollision();
//	//m_pCollision->Collison(COLLISIONTAG::PLAYER);
//	//m_pCollision->setActive(false);
//	//m_pCollision->setHit(false);
//}
//if (m_pAttackColl->getHit())
//{
//	cout << "Player got Hit!" << endl;
//	m_pAttackColl->ResetCollision();
//	m_pAttackColl->setActive(false);
//}