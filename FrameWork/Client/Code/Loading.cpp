#include "pch.h"
#include "Loading.h"

CLoading::CLoading() : m_eSceneID(SCENEID::STAGE_END), m_pDevice(nullptr), m_bFinish(false)
{
	ZeroMemory(m_szLoading, sizeof(_tchar) * 256);
}

CLoading::CLoading(LPDIRECT3DDEVICE9 pDevice) : m_eSceneID(SCENEID::STAGE_END), m_pDevice(pDevice), m_bFinish(false)
{
	m_pDevice->AddRef();
	ZeroMemory(m_szLoading, sizeof(_tchar) * 256);
}

CLoading::~CLoading()
{
}

HRESULT CLoading::Init_Loading(SCENEID eLoading)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eSceneID = eLoading;
	return S_OK;
}

_uint CLoading::Loading_ForStage()
{
	//ToDo:쓰레드로 불러들일 곳
	return _uint();
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pDevice, SCENEID eID)
{
	CLoading* pInstance = new CLoading(pDevice);
	if(FAILED(pInstance->Init_Loading(eID)))
		Safe_Release(pInstance);
	return pInstance;
}

_uint CLoading::Thread_Main(void* pArg)
{
	CLoading* pLoading = (CLoading*)pArg;

	_uint iFlag = 0;

	EnterCriticalSection(pLoading->getCrt());

	switch (pLoading->getLoadingID())
	{
	case SCENEID::STAGE_ONE:
		iFlag = pLoading->Loading_ForStage();
		break;
	case SCENEID::STAGE_TWO:
		break;
	}
	LeaveCriticalSection(pLoading->getCrt());
	return iFlag;
}

void CLoading::Free()
{
	Safe_Release(m_pDevice);

	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);
}
