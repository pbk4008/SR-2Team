#ifndef CubeTex_h__
#define CubeTex_h__


// 0번 : Z-
// 1번 : Z+
// 2번 : X-
// 3번 : X+
// 4번 : Y-
// 5번 : Y+

class CCubeObject final : public CGameObject
{
private:
	explicit CCubeObject();
	explicit CCubeObject(LPDIRECT3DDEVICE9 pDevice);
	explicit CCubeObject(const CCubeObject& rhs);
	virtual ~CCubeObject();

public:
	HRESULT Init_CCubeObject(CUBEINFO tCUBEINFO);
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual CGameObject* Clone_GameObject() override;
	void Set_Path(const _uint& iIndex, std::wstring strFolder, std::wstring strFile);
	void Get_Path(const _uint& iIndex, CString& strFolder, CString& strFile);
	void Linking_Transform(_vec3& vScale, _vec3& vRot, _vec3& vPos);
	void Set_Transform(_vec3& vScale, _vec3& vRot, _vec3& vPos);
	void Linking_CUBEINFO(CUBEINFO* pCubeInfo);
	void Set_CUBEINFO(CUBEINFO* pCubeInfo);
	void Set_Texture(const _uint& iIndex, CTexture* pTexture);
	void Get_Detail(int* pDetail) { *pDetail = m_tCUBEINFO.Detail; }
	void Set_Detail(int pDetail) { m_tCUBEINFO.Detail = pDetail; }
public:
	static CCubeObject* Create(LPDIRECT3DDEVICE9 pDevice, CUBEINFO tCUBEINFO);
private:
	virtual HRESULT Add_Component();
	virtual void Free();
private:
	CQuadCubeTex* m_pCubeTex;
	CUBEINFO m_tCUBEINFO;
	std::array<TEXTUREINFO,6> m_tCubeTextureInfo;
	CString FilterName;
};


#endif // CubeTex_h__
