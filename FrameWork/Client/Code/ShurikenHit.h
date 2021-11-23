#pragma once
#ifndef __SHURIKENHIT_H__
#define __SHURIKENHIT_H__
#include "GameObject.h"
BEGIN(Engine)
class CRcTex;
class CAnimation;
END
class CShurikenHitAnim;
class CShurikenEff final : public CGameObject
{
private:
	explicit CShurikenEff();
	explicit CShurikenEff(LPDIRECT3DDEVICE9 pDevice);
	explicit CShurikenEff(const CShurikenEff& rhs);
	virtual ~CShurikenEff();
public:
	HRESULT Init_ShurikenEff();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual CGameObject* Clone_GameObject();
	virtual void ResetObject();
private:
	void setRotate();
public:
	static CShurikenEff* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	void setStart(const _vec3& vStart);
private:
	CRcTex* m_pBuffer;
	CShurikenHitAnim* m_pAnimation;
	_vec3 m_vStart;
};
#endif