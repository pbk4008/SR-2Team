#pragma once
#ifndef __MonBulletAnim_H__
#define __MonBulletAnim_H__
#include "Animation.h"

class CMonBulletAnim final : public CAnimation
{
private:
	explicit CMonBulletAnim();
	explicit CMonBulletAnim(LPDIRECT3DDEVICE9 pDevice);
	explicit CMonBulletAnim(const CMonBulletAnim& rhs);
	virtual ~CMonBulletAnim();

public:
	HRESULT Init_MonBulletAnim();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual void Render_Animation();
	virtual CComponent* Clone_Component();

public:
	static CMonBulletAnim* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual void ResetTimer();
	virtual void Free();

};
#endif
