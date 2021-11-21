#pragma once
#ifndef __TERRAIN_H__
#define __TERRAIN_H__
#include "GameObject.h"
BEGIN(Engine)
class CTerrainTex;
class CNaviMesh;
END
class CTerrain final : public CGameObject
{
private:
	explicit CTerrain();
	explicit CTerrain(LPDIRECT3DDEVICE9 pDevice);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain();
public:
	HRESULT Init_Terrain();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();
	virtual CGameObject* Clone_GameObject();
public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
public:
	void setTexture(const _tchar* pFineName);
	void setVtxSetting(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwInterVal, const _ulong& dwDetail);
	void LoadTransform(const _vec3& vScale, const _vec3& vRotate, const _vec3 vPosition);
	void Create_NaviMesh();
public:
	CNaviMesh* getNaviMesh() { return m_pNaviMesh; }
private:
	CTerrainTex* m_pBufferCom;
	CTexture* m_pTexture;
	CNaviMesh* m_pNaviMesh;
	_vec3 m_vScale;
	_vec3 m_vRotate;
	_vec3 m_vPosition;
};
#endif