#pragma once
#ifndef __BOMB_H__
#define __BOMB_H__
#include "Bullet.h"
class CBombAnim;
class CBomb final : public CBullet
{
private:
	explicit CBomb();
	explicit CBomb(LPDIRECT3DDEVICE9 pDevice);
	explicit CBomb(const CBomb& rhs);
	virtual ~CBomb();
public:
	HRESULT Init_Bomb();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual CGameObject* Clone_GameObject();
	virtual void ResetObject();
private:
	void Move(const _float& fDeltaTime);
	void CreateFog();
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	static CBomb* Create(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual void setPos(const _vec3& vPos);
private:
	CBombAnim* m_pAnimation;
	_float m_fSpeed;
	_bool m_bJump;
};
#endif