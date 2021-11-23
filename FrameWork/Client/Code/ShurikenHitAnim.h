#pragma once
#ifndef __SHURIKENHITANIM_H__
#define __SHURIKENHITANIM_H__
#include "Animation.h"
class CShurikenHitAnim final : public CAnimation
{
private:
	explicit CShurikenHitAnim();
	explicit CShurikenHitAnim(LPDIRECT3DDEVICE9 pDevice);
	explicit CShurikenHitAnim(const CShurikenHitAnim& rhs);
	virtual ~CShurikenHitAnim();

public:
	HRESULT Init_ShurikenHitAnim();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual void Render_Animation();
	virtual CComponent* Clone_Component();
public:
	static CShurikenHitAnim* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void ResetTimer();
	virtual void Free();

};
#endif