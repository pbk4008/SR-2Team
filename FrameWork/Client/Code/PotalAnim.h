#pragma once
#ifndef __POTALANIM_H__
#define __POTALANIM_H__
#include "Animation.h"
class CPotalAnim final : public CAnimation
{
private:
	explicit CPotalAnim();
	explicit CPotalAnim(LPDIRECT3DDEVICE9 pDevice);
	explicit CPotalAnim(const CPotalAnim& rhs);
	virtual ~CPotalAnim();
public:
	HRESULT Init_PotalAnim();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual void Render_Animation();
	virtual CComponent* Clone_Component();
public:
	static CPotalAnim* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void ResetTimer();
	virtual void Free();
};
#endif