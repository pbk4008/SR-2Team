#pragma once
#ifndef __LODING_H__
#define __LODING_H__
#include "Base.h"
#include "INIManager.h"
class CLoading  final : public CBase
{
private:
	explicit CLoading();
	explicit CLoading(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLoading();
public:
	HRESULT Init_Loading(SCENEID eLoading);
	_uint   Loading_ForStage1();
	_uint   Loading_ForStage2();
	_uint   Loading_ForStage3();
	_uint   Loading_ForBossStage();

public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pDevice, SCENEID eID);
	static unsigned CALLBACK Thread_Main(void* pArg);
	HRESULT Load_Terrain(const _tchar* strName);
	HRESULT Load_Quad(const _tchar* strName);
	HRESULT Load_Cube(const _tchar* strName);
	HRESULT Load_Item(const _tchar* strName);
	HRESULT	Load_MeleeMon(const _tchar* strName);
	HRESULT	Load_ShootMon(const _tchar* strName);
	HRESULT	Load_FlyMon(const _tchar* strName);

	HRESULT	Load_Monster(const _tchar* strName);
	HRESULT	Load_Boss(const _tchar* strName);
	
private:
	template<typename ... Args>
	std::string string_format(const std::string& format, Args ... args)
	{
		int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
		if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
		auto size = static_cast<size_t>(size_s);
		auto buf = std::make_unique<char[]>(size);
		std::snprintf(buf.get(), size, format.c_str(), args ...);
		return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
	}
private:
	virtual void Free();
public:
	SCENEID getLoadingID() const { return m_eSceneID; }
	CRITICAL_SECTION* getCrt() { return &m_Crt; }
	_bool getFinish() { return m_bFinish; }
	const _tchar* getString() { return m_szLoading; }
private:
	HANDLE m_hThread;
	CRITICAL_SECTION m_Crt;

	SCENEID m_eSceneID;
	LPDIRECT3DDEVICE9 m_pDevice;
	_bool m_bFinish;
	_tchar m_szLoading[256];

	INIManager* m_pIniManager;
	CTextureMgr* m_pTextureMgr;
};
#endif