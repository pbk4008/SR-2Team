#pragma once
#ifndef Fireball_h__
#define Fireball_h__

#include "Bullet.h"
BEGIN(Engine)
class CCollision;
END
class CFireball_Anim;;
class CFireball : public CBullet
{
private:
	explicit CFireball();
	explicit CFireball(LPDIRECT3DDEVICE9 pDevice);
	explicit CFireball(const CFireball& rhs);
	virtual ~CFireball();

public:
	HRESULT Init_Fireball();
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
	static CFireball* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	CFireball_Anim* m_pAnimation;
	CSphereCollision* m_pCollision;

	_vec3			m_vTargetPos;

	_float			m_fSpeed;

	//반사체크
	_bool m_bReflect;
};

#endif // Fireball_h__
