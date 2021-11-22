#pragma once
#ifndef __POTAL_H__
#define __POTAL_H__
#include "GameObject.h"
BEGIN(Engine)
class CRcTex;
class CTexture;
END
class CPotalAnim;
class CPotal final : public CGameObject
{
private:
	explicit CPotal();
	explicit CPotal(LPDIRECT3DDEVICE9 pDevice);
	explicit CPotal(const CPotal& rhs);
	virtual ~CPotal();
public:
	HRESULT Init_Potal();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual CGameObject* Clone_GameObject();
	virtual void ResetObject();
private:
	void setAnimation();
	void setCollision();
public:
	static CPotal* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	_bool getClear() { return m_bClear; }
private:
	CRcTex* m_pBuffer;
	CPotalAnim* m_pAnimation;
	CSphereCollision* m_pInteract;

	_bool m_bClear;
};
#endif