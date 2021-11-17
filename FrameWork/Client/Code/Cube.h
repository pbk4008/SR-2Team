#pragma once
#ifndef __CUBE_H__
#define __CUBE_H__
#include "GameObject.h"
#include "Quad.h"

class CCube final : public CGameObject
{
private:
	explicit CCube();
	explicit CCube(LPDIRECT3DDEVICE9 pDevice);
	explicit CCube(const CCube& rhs);
	virtual ~CCube();
public:
	HRESULT Init_Cube();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual CGameObject* Clone_GameObject();
	virtual void ResetObject();
public:
	static CCube* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	void setTexture(const _tchar* pTextureName, const _int iIndex);
private:
	vector<CTexture*> m_CubeTexture;
	vector<CRcTex*> m_CubePlane;
};
#endif