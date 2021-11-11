#ifndef QuadTex_h__
#define QuadTex_h__

#include "GameObject.h"

class CQuadObject final : public CGameObject
{
private:
	explicit CQuadObject();
	explicit CQuadObject(LPDIRECT3DDEVICE9 pDevice);
	explicit CQuadObject(const CQuadObject& rhs);
	virtual ~CQuadObject();

public:
	HRESULT Init_CQuadObject();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual CGameObject* Clone_GameObject() override;
	void Set_Path(TCHAR* strFolder, TCHAR* strFile);
	void Get_Path(CString& strFolder, CString& strFile);
	void Linking_Transform(_vec3& vScale, _vec3& vRot, _vec3& vPos);
	void Set_Transform(_vec3& vScale, _vec3& vRot, _vec3& vPos);
	void Linking_QuadInfo(QUADINFO* pTerrainInfo);
	void Set_QuadInfo(QUADINFO* pTerrainInfo);
	void Set_Texture(CTexture* pTexture);
	void Get_Detail(int* pDetail) { *pDetail = m_tQuadInfo.Detail; }
	void Set_Detail(int pDetail) { m_tQuadInfo.Detail = pDetail; }
public:
	static CQuadObject* Create(LPDIRECT3DDEVICE9 pDevice/*, QUADINFO tQuadInfo*/);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
private:
	TCHAR* strTextureFolder;
	TCHAR* strTextureName;
	QUADINFO m_tQuadInfo;
	CRcTex* m_pQuadTex;
	CTexture* m_pTexture;
	CString FilterName;
};



#endif // QuadTex_h__
