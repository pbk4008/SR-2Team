#pragma once
#ifndef __PLAYERMODLE_H__
#define __PLAYERMODLE_H__
#include "GameObject.h"
#include "Player.h"

BEGIN(Engine)
class CCollision;
END
class CPlayerModel final : public CGameObject
{
private:
	explicit CPlayerModel();
	explicit CPlayerModel(LPDIRECT3DDEVICE9 pDevice);
	explicit CPlayerModel(const CPlayerModel& rhs);
	virtual ~CPlayerModel();
public:
	HRESULT Init_PlayerModel();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual CGameObject* Clone_GameObject();
	HRESULT SettingAnimator();
	CPlayer::STATE Act();
private:
	void Change_AnimTexture();
	void Changing(const _float& fDeltaTime);
public:
	static CPlayerModel* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	_bool getChange() { return m_bChange; }
public:
	void setTarget(CTransform* pTarget);
	void setState(CPlayer::STATE eState);
	void setAttackType(CPlayer::ATTACKTYPE eType);
private:
	_float m_fChangeTime;
	_bool m_bChange;
	_bool m_bAttack;
	CPlayer::STATE m_eState;
	CPlayer::ATTACKTYPE m_eAttackType;

	CRcTex* m_pBufferCom;
	CAnimator* m_pAnimator;
};
#endif