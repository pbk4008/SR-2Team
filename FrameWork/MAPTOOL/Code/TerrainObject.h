#ifndef TerrainObject_h__
#define TerrainObject_h__

#include "ToolGameObject.h"

class CTerrainObject final : public CToolGameObject
{
private:
	explicit CTerrainObject();
	explicit CTerrainObject(LPDIRECT3DDEVICE9 pDevice);
	explicit CTerrainObject(const CTerrainObject& rhs);
	virtual ~CTerrainObject();
public:
	HRESULT Init_CTerrainObject(VTXINFO tVtxInfo);
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual CGameObject* Clone_GameObject() override;
	CTerrainTex* Get_Tex() { return m_pTerrainTex; }
public:
	static CTerrainObject* Create(LPDIRECT3DDEVICE9 pDevice,VTXINFO tVtxInfo);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
private:
	CTerrainTex* m_pTerrainTex;

};


#endif // TerrainObject_h__
