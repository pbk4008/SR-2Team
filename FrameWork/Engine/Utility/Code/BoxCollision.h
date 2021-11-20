#pragma once
#ifndef __BOXCOLLISION_H__
#define __BOXCOLLISION_H__
#include "Collision.h"
BEGIN(Engine)
class ENGINE_DLL CBoxCollision final : public CCollision
{
private:
	explicit CBoxCollision();
	explicit CBoxCollision(LPDIRECT3DDEVICE9 pDevice);
	explicit CBoxCollision(const CBoxCollision& rhs);
	virtual ~CBoxCollision();
public:
	HRESULT Init_BoxCollision();
	virtual _int Update_Component(const _float& fDeltaTime);
	void Render_Collision();
	void Collison(COLLISIONTAG eTag);
	virtual CComponent* Clone_Component()override;
public:
	static CBoxCollision* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
public:
	_vec3 getAxis() { return m_vAxis; }
public:
	void setAxis(const _vec3& vAxis);
private:
	_vec3 m_vAxis;
	LPD3DXMESH m_pBox;
};
END
#endif