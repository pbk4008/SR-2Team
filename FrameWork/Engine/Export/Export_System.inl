HRESULT Init_GraphicDev(WINDOW eMode, const _long& dwCntX, const _long& dwCntY, HWND hWnd, CGraphicDev** pDevice)
{
	return CGraphicDev::GetInstance()->Init_GraphicDev(eMode, dwCntX, dwCntY, hWnd, pDevice);
}
void Render_Begin(D3DXCOLOR color)
{
	CGraphicDev::GetInstance()->Render_Begin(color);
}
void Render_End()
{
	CGraphicDev::GetInstance()->Render_End();
}

HRESULT Init_TimeMgr()
{
	return CTimeMgr::GetInstance()->Init_TimeMgr();
}
void Update_TimeMgr()
{
	return CTimeMgr::GetInstance()->Update_TimeMgr();
}
_float GetDeltaTime()
{
	return CTimeMgr::GetInstance()->getDeltaTime();
}

HRESULT Init_FrameMgr(const _float& fLimitFrame)
{
	return CFrameMgr::GetInstance()->Init_FrameMgr(fLimitFrame);
}
_bool IsPermit(const _float& fDeltaTime)
{
	return CFrameMgr::GetInstance()->IsPermit(fDeltaTime);
}

_float GetOutDeltaTime()
{ return CFrameMgr::GetInstance()->getOutDeltaTime(); }

void Update_InputDev()
{
	CInputDev::GetInstance()->Update_InputDev();
}
_bool Key_Up(_ulong dwKey)
{
	return CInputDev::GetInstance()->Key_Up(dwKey);
}
_bool Key_Down(_ulong dwKey)
{
	return CInputDev::GetInstance()->Key_Down(dwKey);
}
_bool Key_Pressing(_ulong dwKey)
{
	return CInputDev::GetInstance()->Key_Pressing(dwKey);
}
_vec3 MousePos(HWND _hWnd)
{
	return CInputDev::GetInstance()->MousePos(_hWnd);
}

void  SetFirstMousePos(HWND _hWnd)
{
	return CInputDev::GetInstance()->SetFirstMousePos(_hWnd);
}

void System_Release()
{
	CInputDev::DestroyInstance();
	CFrameMgr::DestroyInstance();
	CTimeMgr::DestroyInstance();
	CGraphicDev::DestroyInstance();
}