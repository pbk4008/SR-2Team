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
	void ResetCollision();
public:
	static CCollision* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
public:
	COLLISIONTAG getTag();
	_bool getHit() { return m_bHit; }
	_vec3& getCenter() { return m_vCenter; }
	_float& getRadius() { return m_fRadius; }
	COLLISIONTRIGGER getTrigger() { return m_eTrigger; }
public:
	void setCenter(const _vec3& pCenter);
	void setRadius(const _float& fRadius);
	void setTransform(CTransform* pTransform);
	void setTag(COLLISIONTAG eTag);
	void setHit(_bool bHit) { m_bHit = bHit; }
	void setTrigger(COLLISIONTRIGGER eTrigger) { m_eTrigger = eTrigger; }
private:
	CTransform* m_pTransform;
	_vec3 m_vCenter;
	_float m_fRadius;
	COLLISIONTAG m_eTag;
	COLLISIONTRIGGER m_eTrigger;
	LPD3DXMESH m_pSphere;
	_bool m_bHit;
	CCollision* m_pCollider;
	CCollisionMgr* m_pCollisionMgr;
};
END
#endif