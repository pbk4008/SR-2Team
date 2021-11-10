#pragma once
#ifndef __LODING_H__
#define __LODING_H__
#include "Base.h"
class CLoading  final : public CBase
{
private:
	explicit CLoading();
	explicit CLoading(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLoading();
public:
	HRESULT Init_Loading(SCENEID eLoading);
	_uint   Loading_ForStage();
public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pDevice, SCENEID eID);
	static unsigned CALLBACK Thread_Main(void* pArg);
private:
	virtual void Free();
public:
	SCENEID getLoadingID() const { return m_eSceneID; }
	CRITICAL_SECTION* getCrt() { return &m_Crt; }
	_bool getFinish() { return m_bFinish; }
	const _tchar* getString() { return m_szLoading; }
	HRESULT Load_Terrain(const _tchar* strPath, const _tchar* TerrainName);
private:
	HANDLE m_hThread;
	CRITICAL_SECTION m_Crt;

	SCENEID m_eSceneID;
	LPDIRECT3DDEVICE9 m_pDevice;
	_bool m_bFinish;
	_tchar m_szLoading[256];

	CTextureMgr* m_pTextureMgr;
};
#endif