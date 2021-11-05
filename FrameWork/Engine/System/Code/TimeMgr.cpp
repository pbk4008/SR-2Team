#include "Engine_Include.h"
#include "TimeMgr.h"
IMPLEMENT_SINGLETON(CTimeMgr)
CTimeMgr::CTimeMgr() : m_tStatTime(chrono::steady_clock::duration::zero()), m_tEndTime(chrono::steady_clock::duration::zero()), m_fDeltaTime(0.f)
{
}
CTimeMgr::~CTimeMgr()
{
}

HRESULT CTimeMgr::Init_TimeMgr()
{

	m_tStatTime = chrono::steady_clock::now();
	m_tEndTime = chrono::steady_clock::now();
	return S_OK;
}

void CTimeMgr::Update_TimeMgr()
{
	m_tEndTime = chrono::steady_clock::now();
	m_fDeltaTime = chrono::duration<_float>(m_tEndTime - m_tStatTime).count();
	m_tStatTime = m_tEndTime;
}

void CTimeMgr::Free()
{
}
