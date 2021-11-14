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
	CCollision* getCollider() { return m_pCollider; }
public:
	void setCenter(const _vec3& pCenter);
	void setRadius(const _float& fRadius);
	void setTransform(CTransform* pTransform);
	void setTag(COLLISIONTAG eTag);
	void setHit(_bool bHit) { m_bHit = bHit; }
	void setTrigger(COLLISIONTRIGGER eTrigger) { m_eTrigger = eTrigger; }
	void setCollider(CCollision* pCollider) { m_pCollider = pCollider; }
	void setPivot(const _float& fLen) { m_fPivotLen = fLen; }
	void setMaterial(const _float& fR, const _float& fG,const _float& fB,const _float& fA)
	{ m_pMaterial->Diffuse.a = fA; 
	 m_pMaterial->Diffuse.r = fR; 
	 m_pMaterial->Diffuse.g = fG; 
	 m_pMaterial->Diffuse.b = fB; }
private:
	CTransform* m_pTransform;
	_vec3 m_vCenter;
	_float m_fRadius;
	COLLISIONTAG m_eTag;
	COLLISIONTRIGGER m_eTrigger;
	LPD3DXMESH m_pSphere;
	_bool m_bHit;
	_float m_fPivotLen;
	CCollision* m_pCollider;
	CCollisionMgr* m_pCollisionMgr;
	D3DMATERIAL9* m_pMaterial;
};
END
#endif