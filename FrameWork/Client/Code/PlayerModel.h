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
public:
	static CPlayerModel* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	void setTarget(CTransform* pTarget);
	void setState(CPlayer::STATE eState);
private:
	CPlayer::STATE m_eState;

	CRcTex* m_pBufferCom;
	CAnimator* m_pAnimator;
	CCollision* m_pCollision;
	
};
#endif