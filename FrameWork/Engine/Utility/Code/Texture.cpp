#include "Engine_Include.h"
#include "Texture.h"

CTexture::CTexture()
{
}

CTexture::CTexture(LPDIRECT3DDEVICE9 pDevice) : CComponent(pDevice)
{
}

CTexture::CTexture(const CTexture& rhs) : CComponent(rhs)
,m_vecTexture(rhs.m_vecTexture)
{
	_uint iSize = m_vecTexture.size();
	m_vecTexture.reserve(iSize);

	for (auto& iter : m_vecTexture)
		iter->AddRef();
}

CTexture::~CTexture()
{
}

HRESULT CTexture::Init_Texture(vector<LPDIRECT3DBASETEXTURE9>* pTexture)
{
	NULL_CHECK_RETURN(pTexture, E_FAIL);
	m_vecTexture = *pTexture;

	_int iSize = pTexture->size();
	m_vecTexture.reserve(iSize);
	for (auto iter : *pTexture)
	{
		iter->AddRef();
		m_vecTexture.emplace_back(iter);
	}
	return S_OK;
}

_int CTexture::Update_Component(const _float& fDeltaTime)
{
	return 0;
}

void CTexture::Render_Texture(const _uint& iIndex)
{
	if (m_vecTexture.size() < iIndex)
		return;
	m_pDevice->SetTexture(0, m_vecTexture[iIndex]);
}

CComponent* CTexture::Clone_Component()
{
	return new CTexture(*this);
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pDevice, vector<LPDIRECT3DBASETEXTURE9>* pTexture)
{
	CTexture* pInstance = new CTexture(pDevice);
	if (FAILED(pInstance->Init_Texture(pTexture)))
		Safe_Release(pInstance);
	return pInstance;
}


void CTexture::Free()
{
	for_each(m_vecTexture.begin(), m_vecTexture.end(), DeleteObj);
	m_vecTexture.clear();
	m_vecTexture.shrink_to_fit();
	CComponent::Free();
}

LPDIRECT3DTEXTURE9 CTexture::getTexture(const _uint& iCnt)
{
	if (m_vecTexture.size() < iCnt)
		return nullptr;

	return static_cast<LPDIRECT3DTEXTURE9>(m_vecTexture[iCnt]);
}
