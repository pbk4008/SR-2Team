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

HRESULT CTexture::Init_Texture(TEXTURETYPE eType, const _tchar* pPath, const _uint& iCnt)
{
	m_vecTexture.reserve(iCnt);

	LPDIRECT3DBASETEXTURE9 pTexture = nullptr;
	for (_uint i = 0; i < iCnt; i++)
	{
		_tchar szFileName[256] = L"";
		wsprintf(szFileName, pPath, i);
		switch (eType)
		{
		case TEXTURETYPE::TEX_NORMAL:
			FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pDevice, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			break;
		case TEXTURETYPE::TEX_CUBE:
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(m_pDevice, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			break;
		case TEXTURETYPE::TEX_HEIGHT:
			FAILED_CHECK_RETURN(D3DXCreateTextureFromFileEx(m_pDevice, szFileName, D3DX_DEFAULT, D3DX_DEFAULT,
				D3DX_DEFAULT, 0,D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0,
				NULL, NULL, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			break;
		}
		m_vecTexture.push_back(pTexture);
	}
	return S_OK;
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

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pDevice, TEXTURETYPE eType, const _tchar* pPath, const _uint& iCnt)
{
	CTexture* pInstance = new CTexture(pDevice);
	if (FAILED(pInstance->Init_Texture(eType, pPath, iCnt)))
		Safe_Release(pInstance);
	return pInstance;
}

void CTexture::Free()
{
	for_each(m_vecTexture.begin(), m_vecTexture.end(), DeleteMap);
	m_vecTexture.clear();
	m_vecTexture.shrink_to_fit();
	CComponent::Free();
}

LPDIRECT3DTEXTURE9 CTexture::getTexture(const _int& iCnt)
{
	if (m_vecTexture.size() < iCnt)
		return nullptr;

	return static_cast<LPDIRECT3DTEXTURE9>(m_vecTexture[iCnt]);
}
