#ifndef TerrainObject_h__
#define TerrainObject_h__

#include "GameObject.h"

class CTerrainObject final : public CGameObject
{
private:
	explicit CTerrainObject();
	explicit CTerrainObject(LPDIRECT3DDEVICE9 pDevice);
	explicit CTerrainObject(const CTerrainObject& rhs);
	virtual ~CTerrainObject();
public:
	HRESULT Init_CTerrainObject(TERRAININFO tTerrainInfo);
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual CGameObject* Clone_GameObject() override;
	void Set_Path(TCHAR* strFolder,TCHAR* strFile);
	void Get_Path(CString& strFolder, CString& strFile);
	void Linking_Transform(_vec3& vScale, _vec3& vRot, _vec3& vPos);
	void Set_Transform(_vec3& vScale, _vec3& vRot, _vec3& vPos);
	void Linking_TerrainInfo(TERRAININFO* pTerrainInfo);
	void Set_TerrainInfo(TERRAININFO* pTerrainInfo);
	void Set_Texture(CTexture* pTexture);
	void Get_Detail(int* pDetail) { *pDetail = m_tTerrainInfo.Detail; }
	void Set_Detail(int pDetail) { m_tTerrainInfo.Detail = pDetail; }
	CTerrainTex* Get_Tex() { return m_pTerrainTex; }
public:
	static CTerrainObject* Create(LPDIRECT3DDEVICE9 pDevice,TERRAININFO tTerrainInfo);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
private:
	TCHAR* strTextureFolder;
	TCHAR* strTextureName;
	TERRAININFO m_tTerrainInfo;
	CTerrainTex* m_pTerrainTex;
	CTexture* m_pTexture;
	
};


#endif // TerrainObject_h__
