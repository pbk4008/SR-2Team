#pragma once
#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CTexture final : public CComponent
{
private:
	explicit CTexture();
	explicit CTexture(LPDIRECT3DDEVICE9 pDevice);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();
public:
	HRESULT Init_Texture();
	virtual _int Update_Component(const _float& fDeltaTime);
	void Render_Texture(const _uint& iIndex = 0);
	void Render_MultiTexture(const _uint& iLimitIndex);
public:
	virtual CComponent* Clone_Component();
public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual void Free();
public:
	LPDIRECT3DTEXTURE9 getTexture(const _uint& iCnt);
	inline _int getTextureCount() { return m_vecTexture.size(); }
public:
	HRESULT setTexture(vector<LPDIRECT3DBASETEXTURE9>* pTexture);
private:
	vector<LPDIRECT3DBASETEXTURE9>	m_vecTexture;
};
END
#endif