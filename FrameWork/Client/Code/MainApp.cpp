#include "pch.h"
#include "MainApp.h"

CMainApp::CMainApp() : m_pGraphicDev(nullptr),m_pDevice(nullptr), m_pManagement(nullptr)
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Init_MainApp()
{
	FAILED_CHECK_RETURN(GraphicDevice_Setting(), E_FAIL);
	FAILED_CHECK_RETURN(Init_Scene(), E_FAIL);
	

	return S_OK;
}

_int CMainApp::Update_MainApp(const float& fDeltaTime)
{
	if (!m_pManagement)
		return -1;
	m_pManagement->Update_Management(fDeltaTime);

	return 0;
}

void CMainApp::LateUpdate_MainApp()
{
	if (!m_pManagement)
		return;
	m_pManagement->LateUpdate_Management();
}

void CMainApp::Render_MainApp()
{
	m_pGraphicDev->Render_Begin(D3DXCOLOR(0.5f,0.5f,0.5f,1.f));

	if (m_pManagement)
		m_pManagement->Render_Manangement(m_pDevice);

	m_pGraphicDev->Render_End();
}

HRESULT CMainApp::GraphicDevice_Setting()
{
	FAILED_CHECK_RETURN(Init_GraphicDev(WINDOW::WIN, WINCX, WINCY, g_hWnd, &m_pGraphicDev), E_FAIL);
	NULL_CHECK_RETURN(m_pGraphicDev, E_FAIL);

	m_pGraphicDev->AddRef();
	m_pDevice = m_pGraphicDev->getDevice();
	m_pDevice->AddRef();
	return S_OK;
}

HRESULT CMainApp::Init_Scene()
{
	CScene* pScene = nullptr;

	FAILED_CHECK_RETURN(Init_Manangement(&m_pManagement), E_FAIL);
	m_pManagement->AddRef();

	//TODO:¾À ¸¸µé±â

	FAILED_CHECK_RETURN(m_pManagement->Change_Scene(pScene),E_FAIL);


	return S_OK;
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
	m_pManagement->DestroyInstance();
	System_Release();
	Utility_Release();
}
