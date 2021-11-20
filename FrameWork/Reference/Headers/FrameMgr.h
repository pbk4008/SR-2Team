#pragma once
#ifndef __FRAMEMGR_H__
#define __FRAMEMGR_H__
#include "Base.h"
BEGIN(Engine)
class ENGINE_DLL CFrameMgr final : public CBase
{
	DECLARE_SINGLETON(CFrameMgr)
private:
	explicit CFrameMgr();
	virtual~CFrameMgr();
public:
	HRESULT Init_FrameMgr(const _float& fLimitFrame);
	_bool IsPermit(const _float& fDeltaTime);
public:
	inline _float getOutDeltaTime() { return m_fOutDeltaTime; }
private:
	virtual void Free();
private:
	_float m_fLimitFrame;
	_float m_fCalculDeltaTime;
	_float m_fFPSTime;
	_float m_fOutDeltaTime;
	_int m_iFPS;
};
END
#endif