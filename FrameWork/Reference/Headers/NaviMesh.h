#pragma once
#ifndef __NAVIMESH_H__
#define __NAVIMESH_H__
#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CNaviMesh final : public CComponent
{
private:
	explicit CNaviMesh();
	explicit CNaviMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CNaviMesh(const CNaviMesh& rhs);
	virtual ~CNaviMesh();
public:
	HRESULT Init_Navimesh(const _ulong& dwCntX, const _ulong& dwCntZ);
	virtual _int Update_Component(const _float& fDeltaTime);
	void Render_NaviMesh();
	virtual CComponent* Clone_Component();
	void Check_Shape(const _vec3& vMinPos, const _vec3& vMaxPos);
	void Connet_NaviMesh();
private:
	void Create_Cell(const _vec3* pVtx, const _float& fY);
public:
	static CNaviMesh* Create(LPDIRECT3DDEVICE9 pDevice, const _ulong& dwCntX, const _ulong& dwCntZ);
private:
	virtual void Free();
public:
	vector<list<CELL*>> getMesh() { return m_vecNaviMesh; }
	vector<CELL*> getNaviCell() { return m_vecNaviCell; }
public:
	void setVtxCnt(const _vec3* pVtx, const _ulong& dwCntX, const _ulong& dwCntZ, const _float& fY);
private:
	vector<CELL*> m_vecNaviCell;
	vector<list<CELL*>> m_vecNaviMesh;
	_ulong m_dwCntX;
	_ulong m_dwCntZ;
};
END
#endif