#pragma once
#ifndef __FOG_H__
#define __FOG_H__
#include "GameObject.h"
class CFogAnim;
class CFog final : public CGameObject
{
private:
	explicit CFog();
	explicit CFog(LPDIRECT3DDEVICE9 pDevice);
	explicit CFog(const CFog& rhs);
	virtual ~CFog();
public:
	HRESULT Init_Fog();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual CGameObject* Clone_GameObject();
	virtual void ResetObject();
private:
	void setSideMatrix();
public:
	static CFog* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	void setPos(const _vec3& vPos) { m_vFirstPos = vPos; }
private:
	CCollision* m_pCollision;
	CFogAnim* m_pAnimation;
	CTransform* m_pSideTransform;
	CRcTex* m_pFrontBuffer;
	CRcTex* m_pSideBuffer;
	_vec3 m_vFirstPos;
	_float m_fDestroyTime;
};
#endif