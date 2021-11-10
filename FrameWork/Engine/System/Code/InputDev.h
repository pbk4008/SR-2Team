#pragma once
#ifndef __INPUTDEV_H__
#define __INPUTDEV_H__
#include "Base.h"
BEGIN(Engine)

constexpr _ulong VIR_W = 0x00000001;
constexpr _ulong VIR_A = 0x00000002;
constexpr _ulong VIR_S = 0x00000004;
constexpr _ulong VIR_D = 0x00000008;
constexpr _ulong VIR_SPACE = 0x00000010;
constexpr _ulong VIR_LBUTTON = 0x00000020;
constexpr _ulong VIR_RBUTTON = 0x00000040;
constexpr _ulong VIR_NUM1 = 0x00000080;
constexpr _ulong VIR_NUM2 = 0x00000100;
constexpr _ulong VIR_NUM3 = 0x00000200;
constexpr _ulong VIR_ENTER = 0x01000000;

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
	_bool No_Key();
	_vec3 MousePos(HWND _hWnd);
	void  SetFirstMousePos(HWND _hWnd);
	void MouseFix(_float fX, _float fY , HWND hWnd);
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