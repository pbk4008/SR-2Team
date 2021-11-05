#include "Engine_Include.h"
#include "InputDev.h"

IMPLEMENT_SINGLETON(CInputDev)
CInputDev::CInputDev() : m_dwKey(0), m_dwKeyDown(0), m_dwKeyUp(0)
{
	ZeroMemory(&m_tMousPos, sizeof(POINT));
}

CInputDev::~CInputDev()
{
}

void CInputDev::Update_InputDev()
{
	m_dwKey = 0;

	if (GetAsyncKeyState('W') & 0x8000)
		m_dwKey |= VIR_W;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwKey |= VIR_A;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwKey |= VIR_S;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwKey |= VIR_D;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwKey |= VIR_ENTER;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= VIR_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKey |= VIR_RBUTTON;
}

_bool CInputDev::Key_Up(_ulong dwKey)
{
	if (m_dwKey & dwKey)
	{
		m_dwKeyUp |= dwKey;
		return false;
	}
	else if (m_dwKeyUp & dwKey)
	{
		m_dwKeyUp ^= dwKey;
		return true;
	}
	return false;
}

_bool CInputDev::Key_Down(_ulong dwKey)
{
	if ((m_dwKey & dwKey) && !(m_dwKeyDown & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}
	else if (!(m_dwKey & dwKey) && (m_dwKeyDown & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}
	return false;
}

_bool CInputDev::Key_Pressing(_ulong dwKey)
{
	if (m_dwKey & dwKey)
		return true;

	return false;
}

_vec3 CInputDev::MousePos(HWND _hWnd)
{
	POINT pt = {};
	GetCursorPos(&pt);

	_vec3 vDir = _vec3(_float(pt.x - m_tMousPos.x) ,_float(pt.y-m_tMousPos.y),0.f);
	//D3DXVec3Normalize(&vDir,&vDir);

	m_tMousPos = pt;

	return vDir;
}

void CInputDev::SetFirstMousePos(HWND _hWnd)
{
	GetCursorPos(&m_tMousPos);
}

void CInputDev::Free()
{
}
