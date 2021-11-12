#pragma once
#ifndef __FlyMon_Idle_h__
#define __FlyMon_Idle_h__

#include "Animation.h"

class CFlyMon_Idle : public CAnimation
{
private:
	explicit CFlyMon_Idle();
	explicit CFlyMon_Idle(LPDIRECT3DDEVICE9 pDevice);
	explicit CFlyMon_Idle(const CAnimation& rhs);
	virtual ~CFlyMon_Idle();

public:
	HRESULT Init_FlyMon_Idle();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();

public:
	static CFlyMon_Idle* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual void Free();
};

#endif // FlyMon_Idle_h__