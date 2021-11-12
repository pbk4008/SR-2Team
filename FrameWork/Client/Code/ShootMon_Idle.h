#pragma once
#ifndef __ShootMon_Idle_h__
#define __ShootMon_Idle_h__

#include "Animation.h"

class CShootMon_Idle : public CAnimation
{
private:
	explicit CShootMon_Idle();
	explicit CShootMon_Idle(LPDIRECT3DDEVICE9 pDevice);
	explicit CShootMon_Idle(const CAnimation& rhs);
	virtual ~CShootMon_Idle();

public:
	HRESULT Init_ShootMon_Idle();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();

public:
	static CShootMon_Idle* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	virtual void Free();
};

#endif // ShootMon_Idle_h__