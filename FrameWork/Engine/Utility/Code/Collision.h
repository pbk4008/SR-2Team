#pragma once
#ifndef __COLLISION_H__
#define __COLLISION_H__
#include "Component.h"
BEGIN(Engine)
class CTransform;
class CCollisionMgr;
class CGameObject;
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
public:
	void setTransform(const _vec3& pCenter);
	void setTransform(CTransform* pTransform);
	void setTag(COLLISIONTAG eTag);
	void setHit(_bool bHit) { m_bHit = bHit; }
	void setTrigger(COLLISIONTRIGGER eTrigger) { m_eTrigger = eTrigger; }
	void setCollider(CCollision* pCollider) { m_pCollider = pCollider; }
	void setCenter(const _vec3& vCenter) { m_vCenter = vCenter; }
	void setPivot(const _vec3& vPivot) { m_vPivot = vPivot; }
	void setPivot(const _float& fPivotLen) { m_fPivotLen = fPivotLen; };
	void setTarget(CGameObject* pTarget);
protected:
	CGameObject* m_pTarget;
	CTransform* m_pTransform;
	CCollision* m_pCollider;
	CCollisionMgr* m_pCollisionMgr;
	COLLISIONTAG m_eTag;
	COLLISIONTRIGGER m_eTrigger;
	_bool m_bHit;
	_vec3 m_vPivot;
	_float m_fPivotLen;
	_vec3 m_vCenter;
};
END
#endif