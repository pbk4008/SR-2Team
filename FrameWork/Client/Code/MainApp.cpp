#include "pch.h"
#include "MainApp.h"

CMainApp::CMainApp() : m_pGraphicDev(nullptr),m_pDevice(nullptr)
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Init_MainApp()
{
	Init_GraphicDev(WINDOW::WIN, WINCX, WINCY, g_hWnd, &m_pGraphicDev);

	NULL_CHECK_RETURN(m_pGraphicDev, E_FAIL);

	m_pGraphicDev->AddRef();
	m_pDevice = m_pGraphicDev->getDevice();
	m_pDevice->AddRef();

	return S_OK;
}

_int CMainApp::Update_MainApp(const float& fDeltaTime)
{
	return 0;
}

void CMainApp::LateUpdate_MainApp()
{
}

void CMainApp::Render_MainApp()
{
	m_pGraphicDev->Render_Begin(D3DXCOLOR(0.5f,0.5f,0.5f,1.f));
	m_pGraphicDev->Render_End();
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;
	if (FAILED(pInstance->Init_MainApp()))
		Safe_Release(pInstance);
	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pGraphicDev);

	System_Release();
}
