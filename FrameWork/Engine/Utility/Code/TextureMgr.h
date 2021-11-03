#pragma once
#ifndef __TEXTUREMGR_H__
#define __TEXTUREMGR_H__
#include "Base.h"
BEGIN(Engine)
class ENGINE_DLL CTextureMgr final : CBase
{
DECLARE_SINGLETON(CTextureMgr)
private:
	explicit CTextureMgr();
	virtual ~CTextureMgr();
public:
	HRESULT Insert_Texture(LPDIRECT3DDEVICE9 pDevice, TEXTURETYPE eType, const _tchar* pPath, const _tchar* pState, const _uint& iCnt);
private:
	vector<LPDIRECT3DBASETEXTURE9>* Find_Texture(TEXTURETYPE eType, const _tchar* pState);
public:
	vector<LPDIRECT3DBASETEXTURE9>* getTexture(const _tchar* pTag, TEXTURETYPE eType);
private:
	virtual void Free();
private:
	//key값으로 상태
	//ex)Player_Idle
	map<const _tchar*, vector<LPDIRECT3DBASETEXTURE9>> m_mapTexture[(_ulong)TEXTURETYPE::TEX_END];
};
END
#endif