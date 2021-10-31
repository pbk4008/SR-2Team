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

void System_Release()
{
	CGraphicDev::DestroyInstance();
}