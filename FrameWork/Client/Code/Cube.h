#pragma once
#ifndef __CUBE_H__
#define __CUBE_H__
#include "GameObject.h"
#include "Quad.h"
BEGIN(Engine)
class CBoxCollision;
END
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
	void LoadTransform(const _vec3& vScale, const _vec3& vRotate, const _vec3 vPosition);
public:
	static CCube* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	void setTexture(const _tchar* pTextureName, const _int iIndex);
private:
	vector<CTexture*> m_CubeTexture;
	CCubeTex* m_CubePlane;

	CBoxCollision* m_pCollision;
	_vec3 m_vScale;
	_vec3 m_vRotate;
	_vec3 m_vPosition;
};
#endif