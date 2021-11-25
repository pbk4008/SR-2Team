#pragma once
#ifndef __SHURIKEN_H__
#define __SHURIKEN_H__
#include "Bullet.h"
BEGIN(Engine)
class CSphereCollision;
END
class CShurikenAnim;
class CShurikenEff;
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
	void setEffect();
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	virtual void setPos(const _vec3& vPos);
	void setScene(SCENEID eScene) { m_eScene = eScene; }
public:
	static CShuriken* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	CShurikenAnim* m_pAnimation;
	CSphereCollision* m_pCollision;

	CShurikenEff* m_pEffect;
	_float m_fSpeed;

	SCENEID m_eScene;
};
#endif