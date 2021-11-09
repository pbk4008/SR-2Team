#pragma once
#ifndef __COLLISION_H__
#define __COLLISION_H__
#include "Component.h"
BEGIN(Engine)
class CTransform;
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
public:
	static CCollision* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
public:
	void setCenter(const _vec3& pCenter);
	void setRadius(const _float& fRadius);
	void setTransform(CTransform* pTransform);
private:
	CTransform* m_pTransform;
	_vec3 m_vCenter;
	_float m_fRadius;
};
END
#endif