#include "pch.h"
#include "CubeObject.h"

CCubeObject::CCubeObject()
	: m_pCubeTex(nullptr)
{
}

CCubeObject::CCubeObject(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
	, m_pCubeTex(nullptr)
{

}

CCubeObject::CCubeObject(const CCubeObject& rhs)
	: CGameObject(rhs)
	, m_pCubeTex(rhs.m_pCubeTex)
	, m_tCUBEINFO(rhs.m_tCUBEINFO)
	, m_tCubeTextureInfo(rhs.m_tCubeTextureInfo)
{
}

CCubeObject::~CCubeObject()
{

}

HRESULT CCubeObject::Init_CCubeObject(CUBEINFO tCUBEINFO)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CCubeObject::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);

	return iExit;
}

void CCubeObject::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();

}

void CCubeObject::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &(m_pTransform->getWorldMatrix()));

	

	for(_uint i = 0 ; i < m_tCubeTextureInfo.size() ; ++i)
	{
		if (m_tCubeTextureInfo[i].pTexture)
			m_tCubeTextureInfo[i].pTexture->Render_MultiTexture(i);
	}

	m_pCubeTex->Render_Buffer();

	CGameObject::Render_GameObject();
}

Engine::CGameObject* CCubeObject::Clone_GameObject()
{
	return new CCubeObject(*this);
}

void CCubeObject::Set_Path(const _uint& iIndex, std::wstring strFolder, std::wstring strFile)
{
	if (m_tCubeTextureInfo[iIndex].strTextureFolder == strFolder &&
		m_tCubeTextureInfo[iIndex].strTextureName == strFile)
		return;

	m_tCubeTextureInfo[iIndex].strTextureFolder.clear();
	m_tCubeTextureInfo[iIndex].strTextureName.clear();
	m_tCubeTextureInfo[iIndex].strTextureFolder = strFolder;
	m_tCubeTextureInfo[iIndex].strTextureName = strFile;

}

void CCubeObject::Get_Path(const _uint& iIndex, CString& strFolder, CString& strFile)
{
	strFolder = m_tCubeTextureInfo[iIndex].strTextureFolder.c_str();
	strFile = m_tCubeTextureInfo[iIndex].strTextureName.c_str();

}

void CCubeObject::Linking_Transform(_vec3& vScale, _vec3& vRot, _vec3& vPos)
{
	vScale = m_pTransform->getScale();
	vRot = m_pTransform->getToolAngle();
	vPos = m_pTransform->getPos();
}

void CCubeObject::Set_Transform(_vec3& vScale, _vec3& vRot, _vec3& vPos)
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

void CCubeObject::Linking_CUBEINFO(CUBEINFO* pCubeInfo)
{
	*pCubeInfo = m_tCUBEINFO;

}

void CCubeObject::Set_CUBEINFO(CUBEINFO* pCubeInfo)
{
	m_tCUBEINFO = *pCubeInfo;
}

void CCubeObject::Set_Texture(const _uint& iIndex, CTexture* pTexture)
{
	
	if (m_tCubeTextureInfo[iIndex].pTexture)
		Safe_Release(m_tCubeTextureInfo[iIndex].pTexture);

	m_tCubeTextureInfo[iIndex].pTexture = pTexture;
}

CCubeObject* CCubeObject::Create(LPDIRECT3DDEVICE9 pDevice, CUBEINFO tCUBEINFO)
{
	CCubeObject* pInstance = new CCubeObject(pDevice);
	if (FAILED(pInstance->Init_CCubeObject(tCUBEINFO)))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CCubeObject::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	pComponent = m_pCubeTex = CQuadCubeTex::Create(m_pDevice);
	m_mapComponent->emplace(COMPONENTID::QUADCUBETEX, pComponent);

	return S_OK;
}

void CCubeObject::Free()
{
	for (auto& Info : m_tCubeTextureInfo)
	{
		Safe_Release(Info.pTexture);
	}
	Safe_Release(m_pCubeTex);
	CGameObject::Free();
}
