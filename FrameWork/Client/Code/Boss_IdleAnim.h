#pragma once
#ifndef __Boss_IdleAnim_h__
#define __Boss_IdleAnim_h__

#include "Animation.h"

class CBoss_IdleAnim : public CAnimation
{
private:
	explicit CBoss_IdleAnim();
	explicit CBoss_IdleAnim(LPDIRECT3DDEVICE9 pDevice);
	explicit CBoss_IdleAnim(const CAnimation& rhs);
	virtual ~CBoss_IdleAnim();

public:
	HRESULT Init_Boss_IdleAnim();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();

public:
	static CBoss_IdleAnim* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual void Free();
};

#endif // Boss_IdleAnim_h__