#pragma once
#ifndef __COMPONENET_H__
#define __COMPONENET_H__
#include "Base.h"
BEGIN(Engine)
class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent();
	explicit CComponent(LPDIRECT3DDEVICE9 pDevice);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent();
protected:
	virtual _int Update_Component();
public:
	virtual CComponent* Clone_Component() PURE;
protected:
	virtual void Free();
protected:
	LPDIRECT3DDEVICE9 m_pDevice;
};
END
#endif