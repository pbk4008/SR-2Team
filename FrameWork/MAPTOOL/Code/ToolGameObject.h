#ifndef ToolGameObject_h__
#define ToolGameObject_h__

#include "GameObject.h"


class CToolGameObject : public CGameObject
{
protected:
	explicit CToolGameObject();
	explicit CToolGameObject(LPDIRECT3DDEVICE9 pDevice);
	explicit CToolGameObject(const CToolGameObject& rhs);
	virtual ~CToolGameObject();
public:
	void				Set_Path(std::wstring strFolder, std::wstring strFile, const _uint& iIndex = 0);
	void				Get_Path(CString& strFolder, CString& strFile, const _uint& iIndex = 0);
	void				Linking_Transform(_vec3& vScale, _vec3& vRot, _vec3& vPos);
	void				Set_Transform(_vec3& vScale, _vec3& vRot, _vec3& vPos);
	void				Linking_VTXINFO(VTXINFO* pVtxInfo);
	inline	void		Set_VTXINFO(VTXINFO* pVtxInfo) { m_tVTXINFO = *pVtxInfo; }
	inline	void		Get_VTXINFO(VTXINFO** pVtxInfo) { *pVtxInfo = &m_tVTXINFO; }
	void				Set_Texture(CTexture* pTexture, const _uint& iIndex = 0);
	inline	void		Get_Detail(int* pDetail) { *pDetail = m_tVTXINFO.Detail; }
	inline	void		Set_Detail(int pDetail) { m_tVTXINFO.Detail = pDetail; }
	inline	void		Set_ObjectName(const CString& rhs) { m_strObjectName = rhs; }
	inline	void		Get_ObjectName(CString& rhs) { rhs = m_strObjectName; }
	inline	void		Get_TypeName(CString& rhs) { rhs = m_strTypeName; };
	inline	void		Set_TypeName(const CString& rhs) { m_strTypeName = rhs; };
	inline	void		Set_TreeName(const std::string& strParent) { m_strParent = strParent; }
	inline	void		Get_TreeName(std::string& strParent) { strParent = m_strParent; }
			_bool		Compare_Info(VTXINFO* pVtxInfo);
			_bool		Compare_Filter(const CString& rhs);
	inline	CTexture*	Get_Texture(const _uint& _index = 0) { return m_vecTextureInfo[_index].pTexture;}
	inline std::vector<TEXTUREINFO>& Get_vecTextureInfo() { return m_vecTextureInfo; }
	inline void Set_vecTextureInfo(const std::vector<TEXTUREINFO>& vecTextureInfo) { std::copy(vecTextureInfo.begin(), vecTextureInfo.end(), m_vecTextureInfo.begin()); }


protected:
	virtual void Free() override;
protected:
	VTXINFO m_tVTXINFO;
	std::vector<TEXTUREINFO> m_vecTextureInfo;
	CString m_strObjectName;
	CString m_strTypeName;
protected: D3DMATERIAL9 mMaterial;
	std::string m_strParent;

};

#endif // ToolGameObject_h__
