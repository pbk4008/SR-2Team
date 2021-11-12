#pragma once
#ifndef __BULLET_H__
#define __BULLET_H__
#include "GameObject.h"
class CBullet abstract : public CGameObject
{

protected:
	explicit CBullet();
	explicit CBullet(LPDIRECT3DDEVICE9 pDevice);
	explicit CBullet(const CBullet& rhs);
	virtual ~CBullet();
public:
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
protected:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	virtual void setPos(const _vec3& vPos);
	void setLook(const _vec3& vLook);
	void setAngle(const _float& fAngle);
protected:
	CRcTex* m_pBuffer;
	_vec3 m_vFirstPos;
	_vec3 m_vLook;
	_float m_fAngle;
	_float m_fDestroyTime;
};
#endif