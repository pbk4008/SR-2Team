#pragma once
#ifndef __MeleeMon_Idle_h__
#define __MeleeMon_Idle_h__

#include "Animation.h"

class CMeleeMon_Idle : public CAnimation
{
private:
	explicit CMeleeMon_Idle();
	explicit CMeleeMon_Idle(LPDIRECT3DDEVICE9 pDevice);
	explicit CMeleeMon_Idle(const CAnimation& rhs);
	virtual ~CMeleeMon_Idle();

public:
	HRESULT Init_MeleeMon_Idle(CTexture* pTexture);
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();

public:
	static CMeleeMon_Idle* Create(LPDIRECT3DDEVICE9 pDevice, CTexture* pTexture);

private:
	virtual void Free();
};

#endif // MeleeMon_Idle_h__