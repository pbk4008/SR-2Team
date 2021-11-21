#pragma once
#ifndef __Fireball_Anim_H__
#define __Fireball_Anim_H__

#include "Animation.h"

class CFireball_Anim final : public CAnimation
{
private:
	explicit CFireball_Anim();
	explicit CFireball_Anim(LPDIRECT3DDEVICE9 pDevice);
	explicit CFireball_Anim(const CFireball_Anim& rhs);
	virtual ~CFireball_Anim();

public:
	HRESULT Init_Fireball_Anim();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual void Render_Animation();
	virtual CComponent* Clone_Component();

public:
	static CFireball_Anim* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual void ResetTimer();
	virtual void Free();

};
#endif
