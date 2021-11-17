#pragma once
#ifndef __QUAD_H__
#define __QUAD_H__
#include "GameObject.h"
class CQuad final : public CGameObject
{
private:
	explicit CQuad();
	explicit CQuad(LPDIRECT3DDEVICE9 pDevice);
	explicit CQuad(const CQuad& rhs);
	virtual ~CQuad();
public:
	HRESULT Init_Quad();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual CGameObject* Clone_GameObject();
	virtual void ResetObject();
public:
	static CQuad* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	void setTexture(const _tchar* pFineName);
private:
	CRcTex* m_pBuffer;
	CTexture* m_pTexture;
};
#endif