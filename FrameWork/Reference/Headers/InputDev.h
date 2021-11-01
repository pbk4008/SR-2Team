#pragma once
#ifndef __INPUTDEV_H__
#define __INPUTDEV_H__
#include "Base.h"
BEGIN(Engine)
constexpr _ulong VIR_W = 0x00000001;
constexpr _ulong VIR_A = 0x00000002;
constexpr _ulong VIR_S = 0x00000004;
constexpr _ulong VIR_D = 0x00000008;
class ENGINE_DLL CInputDev final : public CBase
{
	DECLARE_SINGLETON(CInputDev);
private:
	explicit CInputDev();
	virtual ~CInputDev();
public:
	void Update_InputDev();
	_bool Key_Up(_ulong dwKey);
	_bool Key_Down(_ulong dwKey);
	_bool Key_Pressing(_ulong dwKey);
	_vec3 MousePos(HWND _hWnd);
private:
	virtual void Free();
private:
	_ulong m_dwKey;
	_ulong m_dwKeyDown;
	_ulong m_dwKeyUp;
	POINT m_tMousPos;
};
END
#endif