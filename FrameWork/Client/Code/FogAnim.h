#pragma once
#ifndef __FOGANIM_H__
#define __FOGANIM_H__
#include "Animation.h"
class CFogAnim final : public CAnimation
{
private:
	explicit CFogAnim();
	explicit CFogAnim(LPDIRECT3DDEVICE9 pDevice);
	explicit CFogAnim(const CFogAnim& rhs);
	virtual ~CFogAnim();
public:
	HRESULT Init_FogAnim();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual void Render_Animation();
	virtual CComponent* Clone_Component();
public:
	static CFogAnim* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
private:
	_int m_iRoutin;
};
#endif