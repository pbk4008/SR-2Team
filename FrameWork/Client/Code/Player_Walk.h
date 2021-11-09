#pragma once
#ifndef __PLAYER_WALKANIM_H__
#define __PLAYER_WALKANIM_H__
#include "Animation.h"
class CPlayerWalk final : public CAnimation
{
private:
	explicit CPlayerWalk();
	explicit CPlayerWalk(LPDIRECT3DDEVICE9 pDevice);
	explicit CPlayerWalk(const CPlayerWalk& rhs);
	virtual ~CPlayerWalk();
public:
	HRESULT Init_PlayerWalk(CTexture* pTexture);
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();
private:
	void SettingAnimation(const _float& fDeltaTime);
	void Move(const float& fDeltaTime);
public:
	static CPlayerWalk* Create(LPDIRECT3DDEVICE9 pDevice, CTexture* pTexture);
private:
	virtual void Free();
public:
	void setTransform(CTransform* pTransform);
private:
	CTransform* m_pTransform;
	_float m_fMoveTime;
	_float m_fSpeed;
	_bool m_bLeftRightCheck;
};
#endif