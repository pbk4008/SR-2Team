#include "Engine_Include.h"
#include "GraphicDev.h"

IMPLEMENT_SINGLETON(CGraphicDev)
CGraphicDev::CGraphicDev() : m_pSDK(nullptr), m_pDevice(nullptr)
{}
CGraphicDev::~CGraphicDev()
{
}

HRESULT CGraphicDev::Init_GraphicDev(WINDOW eMode, const _long& dwCntX, const _long& dwCntY, HWND hWnd, CGraphicDev** pDevice)
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	NULL_CHECK_RETURN(m_pSDK, E_FAIL);

	D3DCAPS9 deviceCaps;
	ZeroMemory(&deviceCaps, sizeof(D3DCAPS9));

	FAILED_CHECK_MSG(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &deviceCaps), L"Device Cap Fail");

	_ulong vp = 0;

	if (deviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS d3pp;
	ZeroMemory(&d3pp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameter(d3pp, eMode, dwCntX, dwCntY, hWnd);

	FAILED_CHECK_MSG(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3pp, &m_pDevice), L"Create Device Failed");

	*pDevice = this;

	FAILED_CHECK_MSG(D3DXCreateLine(m_pDevice, &m_pLine), L"Create Line Failed");
	m_pLine->SetWidth(5.f);
	//m_pLine->SetAntialias(true);

	return S_OK;
}

void CGraphicDev::Render_Begin(D3DXCOLOR color)
{
	m_pDevice->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color,1.0f, 0);
	m_pDevice->BeginScene();
}

void CGraphicDev::Render_End()
{
	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

void CGraphicDev::SetParameter(D3DPRESENT_PARAMETERS& d3pp, WINDOW eMode, const _long& dwCntX, const _long& dwCntY, HWND hWnd)
{
	d3pp.BackBufferWidth = dwCntX;
	d3pp.BackBufferHeight = dwCntY;
	d3pp.BackBufferCount = 1;
	d3pp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3pp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3pp.MultiSampleQuality = 0;

	d3pp.EnableAutoDepthStencil = true;
	d3pp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3pp.hDeviceWindow = hWnd;
	d3pp.Windowed = (_bool)eMode;

	d3pp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

void CGraphicDev::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pSDK);
}
