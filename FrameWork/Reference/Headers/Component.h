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
public:
	virtual _int Update_Component(const _float& fDeltaTime);
public:
	virtual CComponent* Clone_Component() PURE;
protected:
	virtual void Free();
public:
	inline _bool getActive() { return m_bActive; }
public:
	inline void setActive(const _bool& bActive) { m_bActive = bActive; }
protected:
	LPDIRECT3DDEVICE9 m_pDevice;
	_bool m_bActive;//Component 활성화 및 비활성화
};
END
#endif