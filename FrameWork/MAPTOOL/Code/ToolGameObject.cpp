#include "pch.h"
#include "ToolGameObject.h"

CToolGameObject::CToolGameObject()
{

}

CToolGameObject::CToolGameObject(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice)
{

}

CToolGameObject::CToolGameObject(const CToolGameObject& rhs)
	: CGameObject(rhs)
	, m_tVTXINFO(rhs.m_tVTXINFO)
	, m_vecTextureInfo(rhs.m_vecTextureInfo)
	, m_strObjectName(rhs.m_strObjectName)
	, m_strTypeName(rhs.m_strTypeName)
	, m_strParent(rhs.m_strParent)
{
	for (auto& texture : m_vecTextureInfo)
	{
		texture.pTexture->AddRef();
	}
}

CToolGameObject::~CToolGameObject()
{

}

void CToolGameObject::Set_Path( std::wstring strFolder, std::wstring strFile, const _uint& iIndex)
{
	if (m_vecTextureInfo[iIndex].strTextureFolder == strFolder &&
		m_vecTextureInfo[iIndex].strTextureName == strFile)
		return;

	m_vecTextureInfo[iIndex].strTextureFolder = strFolder;
	m_vecTextureInfo[iIndex].strTextureName = strFile;
}

void CToolGameObject::Get_Path( CString& strFolder, CString& strFile,  const _uint& iIndex)
{
	if (!m_vecTextureInfo.empty())
	{
		strFolder = m_vecTextureInfo[iIndex].strTextureFolder.c_str();
		strFile = m_vecTextureInfo[iIndex].strTextureName.c_str();
	}
}

void CToolGameObject::Linking_Transform(_vec3& vScale, _vec3& vRot, _vec3& vPos)
{
	vScale = m_pTransform->getScale();
	vRot = m_pTransform->getToolAngle();
	vPos = m_pTransform->getPos();
}

void CToolGameObject::Set_Transform(_vec3& vScale, _vec3& vRot, _vec3& vPos)
{
	m_pTransform->setScale(vScale);

	_matrix matSRP;
	D3DXMatrixIdentity(&matSRP);

	D3DXQUATERNION quatSRP;
	D3DXQuaternionIdentity(&quatSRP);

	D3DXQuaternionRotationYawPitchRoll(&quatSRP, D3DXToRadian(vRot.y), D3DXToRadian(vRot.x), D3DXToRadian(vRot.z));
	D3DXMatrixRotationQuaternion(&matSRP, &quatSRP);

	m_pTransform->setRotate(matSRP);
	m_pTransform->setToolAngle(vRot);


	m_pTransform->setPos(vPos);
}

void CToolGameObject::Linking_VTXINFO(VTXINFO* pVtxInfo)
{
	*pVtxInfo = m_tVTXINFO;
}


void CToolGameObject::Set_Texture( CTexture* pTexture, const _uint& iIndex)
{
	if (m_vecTextureInfo[iIndex].pTexture)
		Safe_Release(m_vecTextureInfo[iIndex].pTexture);

	m_vecTextureInfo[iIndex].pTexture = pTexture;
}



Engine::_bool CToolGameObject::Compare_Info(VTXINFO* pVtxInfo)
{
	if (*pVtxInfo == m_tVTXINFO)
		return true;
	return false;
}

Engine::_bool CToolGameObject::Compare_Filter(const CString& rhs)
{
	if (m_strObjectName == rhs)
		return true;
	return false;
}


void CToolGameObject::Free()
{
	for (auto& iter : m_vecTextureInfo)
		Safe_Release(iter.pTexture);
	m_vecTextureInfo.clear();
	m_vecTextureInfo.shrink_to_fit();
}
