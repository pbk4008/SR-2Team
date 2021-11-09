#pragma once
#ifndef __COLLISION_H__
#define __COLLISION_H__
#include "Component.h"
BEGIN(Engine)
class CTransform;
class CCollisionMgr;
class ENGINE_DLL CCollision final : public CComponent
{
private:
	explicit CCollision();
	explicit CCollision(LPDIRECT3DDEVICE9 pDevice);
	explicit CCollision(const CCollision& rhs);
	virtual ~CCollision();
public:
	HRESULT Init_Collision();
	virtual _int Update_Component(const _float& fDeltaTime);
	virtual CComponent* Clone_Component();
	void Render_Collision();
	void Collison(COLLISIONTAG eTag);
public:
	static CCollision* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
public:
	COLLISIONTAG getTag();
public:
	_bool getHit() { return m_bHit; }
	_vec3& getCenter() { return m_vCenter; }
	_float& getRadius() { return m_fRadius; }
public:
	void setCenter(const _vec3& pCenter);
	void setRadius(const _float& fRadius);
	void setTransform(CTransform* pTransform);
	void setTag(COLLISIONTAG eTag);
	void setHit(_bool bHit) { m_bHit = bHit; }
private:
	CTransform* m_pTransform;
	_vec3 m_vCenter;
	_float m_fRadius;
	COLLISIONTAG m_eTag;
	LPD3DXMESH m_pSphere;
	_bool m_bHit;

	CCollisionMgr* m_pCollisionMgr;
};
END
#endif