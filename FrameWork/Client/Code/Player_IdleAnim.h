#pragma once
#ifndef __PLAYER_IDLE_ANIM_H__
#define __PLAYER_IDLE_ANIM_H__
#include "Animation.h"
class CPlayerIdleAnim final : public CAnimation
{
private:
	explicit CPlayerIdleAnim();
	explicit CPlayerIdleAnim(LPDIRECT3DDEVICE9 pDevice);
	explicit CPlayerIdleAnim(const CAnimation& rhs);
	virtual ~CPlayerIdleAnim();
public:
	HRESULT Init_PlayerIdleAnim();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();
public:
	static CPlayerIdleAnim* Create(LPDIRECT3DDEVICE9 pDevice, CTexture* pTexture);
private:
	virtual void Free();
};
#endif