#pragma once
#ifndef __COLLISION_H__
#define __COLLISION_H__
#include "Component.h"
BEGIN(Engine)
class CTransform;
class CCollisionMgr;
class ENGINE_DLL CCollision abstract : public CComponent
{
protected:
	explicit CCollision();
	explicit CCollision(LPDIRECT3DDEVICE9 pDevice);
	explicit CCollision(const CCollision& rhs);
	virtual ~CCollision();
public:
	HRESULT Init_Collision();
	virtual _int Update_Component(const _float& fDeltaTime);
	void Render_Collision();
	virtual void Collison(COLLISIONTAG eTag);
	void ResetCollision();
	void WallCollision();
protected:
	virtual void Free();
public:
	COLLISIONTAG getTag();
	_bool getHit() { return m_bHit; }
	_vec3& getCenter() { return m_vCenter; }
	COLLISIONTRIGGER getTrigger() { return m_eTrigger; }
	CCollision* getCollider() { return m_pCollider; }
	_ulong getIndex() { return m_dwIndex; }
public:
	void setCenter(const _vec3& pCenter);
	void setTransform(CTransform* pTransform);
	void setTag(COLLISIONTAG eTag);
	void setHit(_bool bHit) { m_bHit = bHit; }
	void setTrigger(COLLISIONTRIGGER eTrigger) { m_eTrigger = eTrigger; }
	void setCollider(CCollision* pCollider) { m_pCollider = pCollider; }
	void setPivot(const _float& fLen) { m_fPivotLen = fLen; }
	void setIndex(const _ulong& dwIndex) { m_dwIndex = dwIndex; }
	void setMaterial(const _float& fR, const _float& fG,const _float& fB,const _float& fA)
	{ m_pMaterial->Diffuse.a = fA; 
	 m_pMaterial->Diffuse.r = fR; 
	 m_pMaterial->Diffuse.g = fG; 
	 m_pMaterial->Diffuse.b = fB; }
protected:
	CTransform* m_pTransform;
	_vec3 m_vCenter;
	COLLISIONTAG m_eTag;
	COLLISIONTRIGGER m_eTrigger;
	_bool m_bHit;
	_float m_fPivotLen;
	CCollision* m_pCollider;
	CCollisionMgr* m_pCollisionMgr;
	D3DMATERIAL9* m_pMaterial;
	_ulong m_dwIndex;
};
END
#endif