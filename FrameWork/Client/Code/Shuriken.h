#pragma once
#ifndef __SHURIKEN_H__
#define __SHURIKEN_H__
#include "Bullet.h"
BEGIN(Engine)
class CCollision;
END
class CShurikenAnim;
class CShuriken final : public CBullet
{
private:
	explicit CShuriken();
	explicit CShuriken(LPDIRECT3DDEVICE9 pDevice);
	explicit CShuriken(const CShuriken& rhs);
	virtual ~CShuriken();
public:
	HRESULT Init_Shuriken();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual CGameObject* Clone_GameObject();
	virtual void ResetObject();
private:
	void Move(const _float& fDeltaTime);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	void setPos(const _vec3& vPos);
	void setAngle(const _float& fAngle);
	void setLook(const _vec3& vLook);
public:
	static CShuriken* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	CShurikenAnim* m_pAnimation;
	CCollision* m_pCollision;
	_vec3 m_vFirstPos;
	_vec3 m_vLook;
	_float m_fDestroyTime;
	_float m_fAngle;
	_float m_fSpeed;
};
#endif