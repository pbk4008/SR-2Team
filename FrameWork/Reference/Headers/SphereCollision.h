#pragma once
#ifndef __SPHERECOLLISION_H__
#define __SPHERECOLLISION_H__
#include "Collision.h"
BEGIN(Engine)
class ENGINE_DLL CSphereCollision final : public CCollision
{
private:
	explicit CSphereCollision();
	explicit CSphereCollision(LPDIRECT3DDEVICE9 pDevice);
	explicit CSphereCollision(const CSphereCollision& rhs);
	virtual ~CSphereCollision();
public:
	HRESULT Init_SphereCollision();
	virtual _int Update_Component(const _float& fDeltaTime);
	void Render_Collision();
	void Collison(COLLISIONTAG eTag);
	virtual CComponent* Clone_Component()override;
public:
	static CSphereCollision* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
public:
	_float getRadius() { return m_fRadius; }
public:
	void setRadius(const _float& fRadius);
private:
	_float m_fRadius;
	LPD3DXMESH m_pSphere;
};
END
#endif