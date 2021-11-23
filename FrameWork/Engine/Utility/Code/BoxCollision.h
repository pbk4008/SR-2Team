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
private:
	void CreateBoxPoint();
public:
	static CBoxCollision* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
public:
	_vec3 getScale() { return m_vScale; }
	_vec3 getAngle() { return m_vAngle; }
	_vec3* getPoints() { return m_vPoint; }
public: void getRotateMatrix(_matrix& world);
public:
	//void setAxis(const _vec3& vAxis);
	void setAxis(const _vec3& vAxis, const _vec3& vAngle);
private:
	_vec3 m_vScale;
	_vec3 m_vAngle;
	_vec3 m_vPoint[8];
	LPD3DXMESH m_pBox;
};
END
#endif