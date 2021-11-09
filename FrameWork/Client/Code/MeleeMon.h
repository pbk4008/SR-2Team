#pragma once
#ifndef __CMELEEMON_H__
#define __CMELEEMON_H__

#include "Monster.h"
#include "Player.h"

class CMeleeMon : public CMonster
{
public:
	enum class STATE {IDLE, WALKING, ATTACK, MAX};
private:
	explicit CMeleeMon();
	explicit CMeleeMon(LPDIRECT3DDEVICE9 pDevice);
	explicit CMeleeMon(const CMeleeMon& rhs);
	virtual ~CMeleeMon();

public:
	HRESULT Init_MeleeMon();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual CGameObject* Clone_GameObject() override;
	HRESULT SettingAnimator();

private:
	void Change_State();
	
public:
	static CMeleeMon* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual HRESULT Add_Component();
	virtual void	Follow(const _float& fDeltaTime);
	virtual void	Attack(const _float& fDeltaTime);
	virtual void	Attack_Dis(const _float& fDeltaTime);
	virtual void	Free();

private:
	CRcTex*			m_pBufferCom;
	CTexture*		m_pTexture;
	CAnimator*		m_pAnimator;

	STATE			m_eCurState;
	STATE			m_ePreState;

	CCollision*		m_pCollision;
	_bool			m_bAttack;
	_bool			m_bMoving;

	_float			m_iTimer;
	_float			m_fSpeed;

	STATE			m_eState;
};
#endif