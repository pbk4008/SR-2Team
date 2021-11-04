#include "Engine_Include.h"
#include "TextureMgr.h"

IMPLEMENT_SINGLETON(CTextureMgr)
CTextureMgr::CTextureMgr()
{
}
CTextureMgr::~CTextureMgr()
{
}
HRESULT CTextureMgr::Insert_Texture(LPDIRECT3DDEVICE9 pDevice,TEXTURETYPE eType, const _tchar* pPath, const _tchar* pState, const _uint& iCnt)
{
	vector<LPDIRECT3DBASETEXTURE9>* vecTemp = Find_Texture(eType, pState);
	if (vecTemp)
		return E_FAIL;
	
	vector<LPDIRECT3DBASETEXTURE9> pVecTexture;
	pVecTexture.reserve(iCnt);

	LPDIRECT3DBASETEXTURE9 pTexture = nullptr;
	for (_uint i = 0; i < iCnt; i++)
	{
		_tchar szFileName[256] = L"";
		wsprintf(szFileName, pPath, i);
		switch (eType)
		{
		case TEXTURETYPE::TEX_NORMAL:
			FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(pDevice, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			break;
		case TEXTURETYPE::TEX_CUBE:
			FAILED_CHECK_RETURN(D3DXCreateCubeTextureFromFile(pDevice, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture), E_FAIL);
			break;
		case TEXTURETYPE::TEX_HEIGHT:
			FAILED_CHECK_RETURN(D3DXCreateTextureFromFileEx(pDevice, szFileName, D3DX_DEFAULT, D3DX_DEFAULT,
				D3DX_DEFAULT, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0,
				NULL, NULL, (LPDIRECT3DTEXTURE9*)&pTexture), E_FAIL);
			break;
		}
		pVecTexture.emplace_back(pTexture);
	}
	m_mapTexture[(_ulong)eType].emplace(pState, pVecTexture);

	return S_OK;
}
vector<LPDIRECT3DBASETEXTURE9>* CTextureMgr::Find_Texture(TEXTURETYPE eType, const _tchar* pState)
{
	auto iter = find_if(m_mapTexture->begin(), m_mapTexture->begin(), CFind_Tag(pState));
	if (iter == m_mapTexture->end())
		return nullptr;

	return &(iter->second);
}

vector<LPDIRECT3DBASETEXTURE9>* CTextureMgr::getTexture(const _tchar* pTag, TEXTURETYPE eType)
{
	vector<LPDIRECT3DBASETEXTURE9>* vecTexture = Find_Texture(eType, pTag);
	NULL_CHECK_RETURN(vecTexture, nullptr);

	return vecTexture;
}

void CTextureMgr::Free()
{
	for (_int i = 0; i < (_int)TEXTURETYPE::TEX_END; i++)
	{
		for (auto vecTexture : m_mapTexture[i])
		{
			for_each(vecTexture.second.begin(), vecTexture.second.end(), DeleteObj);
			vecTexture.second.clear();
			vecTexture.second.shrink_to_fit();
		}
		m_mapTexture[i].clear();
	}
}
