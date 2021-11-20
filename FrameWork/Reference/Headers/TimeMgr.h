#pragma once
#ifndef __TIMEMGR_H__
#define __TIMEMGR_H__
#include "Base.h"
BEGIN(Engine)
class ENGINE_DLL CTimeMgr final : public CBase
{
	DECLARE_SINGLETON(CTimeMgr)
private:
	explicit CTimeMgr();
	virtual~CTimeMgr();
public:
	HRESULT Init_TimeMgr();
	void Update_TimeMgr();
private:
	virtual void Free();
public:
	inline _float getDeltaTime() { return m_fDeltaTime; }
private:
	chrono::steady_clock::time_point m_tStatTime;
	chrono::steady_clock::time_point m_tEndTime;
	_float m_fDeltaTime;
};
END
#endif