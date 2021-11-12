#include "pch.h"
#include "QuadObject.h"

CQuadObject::CQuadObject()
	: strTextureFolder(nullptr)
	, strTextureName(nullptr)
	, m_pTexture(nullptr)
	, m_pQuadTex(nullptr)
{
}

CQuadObject::CQuadObject(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, strTextureFolder(nullptr)
	, strTextureName(nullptr)
	, m_pTexture(nullptr)
	, m_pQuadTex(nullptr)
{
}

CQuadObject::CQuadObject(const CQuadObject& rhs)
	: CGameObject(rhs)
	, strTextureFolder(rhs.strTextureFolder)
	, strTextureName(rhs.strTextureName)
	, m_pTexture(rhs.m_pTexture)
	, m_pQuadTex(rhs.m_pQuadTex)
{
	strTextureFolder = new TCHAR[lstrlen(rhs.strTextureFolder) + 1];
	lstrcpy(strTextureFolder, rhs.strTextureFolder);
	strTextureName = new TCHAR[lstrlen(rhs.strTextureName) + 1];
	lstrcpy(strTextureName, rhs.strTextureName);
}

CQuadObject::~CQuadObject()
{

}

HRESULT CQuadObject::Init_CQuadObject()
{
	m_tQuadInfo.Detail = 1;
	m_tQuadInfo.Interval = 1;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CQuadObject::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);

	return iExit;
}

void CQuadObject::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();

}

void CQuadObject::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &(m_pTransform->getWorldMatrix()));

	if (m_pTexture)
	{
		m_pTexture->Render_Texture();
	}

	m_pQuadTex->Render_Buffer();

	CGameObject::Render_GameObject();
}

Engine::CGameObject* CQuadObject::Clone_GameObject()
{
	return new CQuadObject(*this);

}

void CQuadObject::Set_Path(TCHAR* strFolder, TCHAR* strFile)
{
	//두 문자열같으면 Set안함
	if (!lstrcmp(strFolder, strTextureFolder) && !lstrcmp(strFile, strTextureName))
		return;
	else //다르면 삭제해줌
	{
		Safe_DeleteArr(strTextureFolder);
		Safe_DeleteArr(strTextureName);
	}

	//할당해서 넣어줌
	if (strFolder && strFile)
	{
		strTextureFolder = new TCHAR[lstrlen(strFolder) + 1];
		lstrcpy(strTextureFolder, strFolder);


		strTextureName = new TCHAR[lstrlen(strFile) + 1];
		lstrcpy(strTextureName, strFile);
	}
}

void CQuadObject::Get_Path(CString& strFolder, CString& strFile)
{
	strFolder = strTextureFolder;
	strFile = strTextureName;
}

void CQuadObject::Linking_Transform(_vec3& vScale, _vec3& vRot, _vec3& vPos)
{
	vScale = m_pTransform->getScale();
	vRot = m_pTransform->getToolAngle();
	vPos = m_pTransform->getPos();
}

void CQuadObject::Set_Transform(_vec3& vScale, _vec3& vRot, _vec3& vPos)
{
	m_pTransform->setScale(vScale);

	_matrix matSRP;
	D3DXQUATERNION quatSRP;
	D3DXMatrixIdentity(&matSRP);
	D3DXQuaternionIdentity(&quatSRP);

	D3DXQuaternionRotationYawPitchRoll(&quatSRP, D3DXToRadian(vRot.y), D3DXToRadian(vRot.x), D3DXToRadian(vRot.z));
	D3DXMatrixRotationQuaternion(&matSRP, &quatSRP);

	m_pTransform->setRotate(matSRP);
	m_pTransform->setToolAngle(vRot);
	m_pTransform->setPos(vPos);
}

void CQuadObject::Linking_QuadInfo(QUADINFO* pTerrainInfo)
{
	*pTerrainInfo = m_tQuadInfo;

}

void CQuadObject::Set_QuadInfo(QUADINFO* pTerrainInfo)
{
	m_tQuadInfo = *pTerrainInfo;

}

void CQuadObject::Set_Texture(CTexture* pTexture)
{
	if (m_pTexture)
		Safe_Release(m_pTexture);

	m_pTexture = pTexture;
}

CQuadObject* CQuadObject::Create(LPDIRECT3DDEVICE9 pDevice/*, QUADINFO tQuadInfo*/)
{
	CQuadObject* pInstance = new CQuadObject(pDevice);
	if (FAILED(pInstance->Init_CQuadObject()))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CQuadObject::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	pComponent = m_pQuadTex = CRcTex::Create(m_pDevice);
	m_mapComponent->emplace(COMPONENTID::RCTEX, pComponent);

	return S_OK;
}

void CQuadObject::Free()
{
	Safe_DeleteArr(strTextureFolder);
	Safe_DeleteArr(strTextureName);
	Safe_Release(m_pQuadTex);
	Safe_Release(m_pTexture);
	CGameObject::Free();
}
