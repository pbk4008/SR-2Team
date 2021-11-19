#pragma once
#ifndef __MonBullet_H__
#define __MonBullet_H__
#include "Bullet.h"
BEGIN(Engine)
class CSphereCollision;
END
class CMonBulletAnim;;
class CMonBullet : public CBullet
{
private:
	explicit CMonBullet();
	explicit CMonBullet(LPDIRECT3DDEVICE9 pDevice);
	explicit CMonBullet(const CMonBullet& rhs);
	virtual ~CMonBullet();

public:
	HRESULT Init_MonBullet();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual CGameObject* Clone_GameObject();

private:
	void Move(const _float& fDeltaTime);

private:
	virtual HRESULT Add_Component();
	virtual void Free();

public:
	void setPos(const _vec3& vPos);
	void setLook(const _vec3& vLook);
	void setTarget(const _vec3& vTarget);

public:
	static CMonBullet* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	CMonBulletAnim*	m_pAnimation;
	CSphereCollision*		m_pCollision;

	_vec3			m_vTargetPos;

	_float			m_fSpeed;

	//반사체크
	_bool m_bReflect;
};
#endif // MonBullet_h__
