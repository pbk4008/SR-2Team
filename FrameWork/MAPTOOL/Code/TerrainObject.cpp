#include "pch.h"
#include "TerrainObject.h"

CTerrainObject::CTerrainObject()
	: strTextureFolder(nullptr)
	, strTextureName(nullptr)
	, m_pTexture(nullptr)
{
	ZeroMemory(&m_tTerrainInfo, sizeof(TERRAININFO));
}

CTerrainObject::CTerrainObject(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, strTextureFolder(nullptr)
	, strTextureName(nullptr)
	, m_pTexture(nullptr)
{
	ZeroMemory(&m_tTerrainInfo, sizeof(TERRAININFO));
}

CTerrainObject::CTerrainObject(const CTerrainObject& rhs)
	: CGameObject(rhs)
	, strTextureFolder(rhs.strTextureFolder)
	, strTextureName(rhs.strTextureName)
	, m_pTexture(rhs.m_pTexture)
{
	strTextureFolder = new TCHAR[sizeof(lstrlen(rhs.strTextureFolder)+1)];
	lstrcpy(strTextureFolder, rhs.strTextureFolder);
	strTextureName   = new TCHAR[sizeof(lstrlen(rhs.strTextureName)+1)];
	lstrcpy(strTextureName, rhs.strTextureName);
	memcpy(&m_tTerrainInfo, &rhs.m_tTerrainInfo, sizeof(TERRAININFO));


}

CTerrainObject::~CTerrainObject()
{

}

HRESULT CTerrainObject::Init_CTerrainObject(TERRAININFO tTerrainInfo)
{
	memcpy(&m_tTerrainInfo, &tTerrainInfo,sizeof(TERRAININFO));
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CTerrainObject::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);


	return iExit;
}

void CTerrainObject::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CTerrainObject::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &(m_pTransform->getWorldMatrix()));

	if (m_pTexture)
	{
		m_pTexture->Render_Texture();
	}

	m_pTerrainTex->Render_Buffer();

	CGameObject::Render_GameObject();
}

Engine::CGameObject* CTerrainObject::Clone_GameObject()
{

	return new CTerrainObject(*this);
}

void CTerrainObject::Set_Path(TCHAR* strFolder, TCHAR* strFile)
{
	//두 문자열같으면 Set안함
	if (!lstrcmp(strFolder, strTextureFolder) && !lstrcmp(strFile,strTextureName))
		return;
	else //다르면 삭제해줌
	{
		Safe_DeleteArr(strTextureFolder);
		Safe_DeleteArr(strTextureName);
	}

	//할당해서 넣어줌
	if (strFolder && strFile)
	{
		strTextureFolder = new TCHAR[lstrlen(strFolder)+1];
		lstrcpy(strTextureFolder, strFolder);


		strTextureName = new TCHAR[lstrlen(strFile)+1];
		lstrcpy(strTextureName, strFile);
	}
}

void CTerrainObject::Get_Path(CString& strFolder, CString& strFile)
{
	strFolder = strTextureFolder;
	strFile = strTextureName;
}

void CTerrainObject::Linking_Transform(_vec3& vScale, _vec3& vRot, _vec3& vPos)
{
	vScale = m_pTransform->getScale();
	vRot = m_pTransform->getAngle();
	vPos = m_pTransform->getPos();
}

void CTerrainObject::Set_Transform(_vec3& vScale, _vec3& vRot, _vec3& vPos)
{
	m_pTransform->setScale(vScale);
	m_pTransform->setAngle(  MATRIXINFO::MAT_RIGHT, vRot.x);
	m_pTransform->setAngle(  MATRIXINFO::MAT_UP, vRot.y);
	m_pTransform->setAngle(  MATRIXINFO::MAT_LOOK, vRot.z);
	m_pTransform->setPos(vPos);
}

void CTerrainObject::Linking_TerrainInfo(TERRAININFO* pTerrainInfo)
{
	*pTerrainInfo = m_tTerrainInfo;
}

void CTerrainObject::Set_TerrainInfo(TERRAININFO* pTerrainInfo)
{
	m_tTerrainInfo = *pTerrainInfo;
}

void CTerrainObject::Set_Texture(CTexture* pTexture)
{

	if (m_pTexture)
		Safe_Release(m_pTexture);

	m_pTexture = pTexture;
}

_bool CTerrainObject::Compare_Info(TERRAININFO* pTerrainInfo)
{
	if (*pTerrainInfo == m_tTerrainInfo)
		return true;
	return false;
}

CTerrainObject* CTerrainObject::Create(LPDIRECT3DDEVICE9 pDevice, TERRAININFO tTerrainInfo)
{
	CTerrainObject* pInstance = new CTerrainObject(pDevice);
	if(FAILED(pInstance->Init_CTerrainObject(tTerrainInfo)))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CTerrainObject::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	pComponent = m_pTerrainTex = CTerrainTex::Create(m_pDevice,m_tTerrainInfo.X,m_tTerrainInfo.Z,m_tTerrainInfo.Interval);
	m_mapComponent->emplace(COMPONENTID::TERRAINTEX, pComponent);

	return S_OK;
}

void CTerrainObject::Free()
{
	Safe_DeleteArr(strTextureFolder);
	Safe_DeleteArr(strTextureName);
	Safe_Release(m_pTerrainTex);
	Safe_Release(m_pTexture);
	CGameObject::Free();
}
