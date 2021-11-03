#include "Engine_Include.h"
#include "TextureMgr.h"

CTextureMgr::CTextureMgr()
{
}
CTextureMgr::~CTextureMgr()
{
}
HRESULT CTextureMgr::Insert_Texture(TEXTURETYPE eType, const _tchar* pPath, const _tchar* pState, const _uint& iCnt)
{
	return S_OK;
}
vector<LPDIRECT3DBASETEXTURE9>* CTextureMgr::Find_Texture(TEXTURETYPE eType, const _tchar* pState)
{
	auto iter = find_if(m_mapTexture->begin(), m_mapTexture->begin(), CFind_Tag(pState));
	if (iter == m_mapTexture->end())
		return nullptr;

	return &(iter->second);
}
