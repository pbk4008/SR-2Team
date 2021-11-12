#pragma once
#ifndef __SHURIKENANIM_H__
#define __SHURIKENANIM_H__
#include "Animation.h"
class CShurikenAnim final : public CAnimation
{
private:
	explicit CShurikenAnim();
	explicit CShurikenAnim(LPDIRECT3DDEVICE9 pDevice);
	explicit CShurikenAnim(const CShurikenAnim& rhs);
	virtual ~CShurikenAnim();
public:
	HRESULT Init_ShurikenAnim();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual void Render_Animation();
	virtual CComponent* Clone_Component();
public:
	static CShurikenAnim* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void ResetTimer();
	virtual void Free();
};
#endif
