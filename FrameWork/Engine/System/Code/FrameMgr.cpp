#include "Engine_Include.h"
#include "FrameMgr.h"

IMPLEMENT_SINGLETON(CFrameMgr)
CFrameMgr::CFrameMgr(): m_fCalculDeltaTime(0.f), m_fLimitFrame(0.f)
{

}
CFrameMgr::~CFrameMgr()
{
}

HRESULT CFrameMgr::Init_FrameMgr(const _float& fLimitFrame)
{
	m_fLimitFrame = 1 / fLimitFrame;
	return S_OK;
}

_bool CFrameMgr::IsPermit(const _float& fDeltaTime)
{
	m_fCalculDeltaTime += fDeltaTime;
	if (m_fLimitFrame <= m_fCalculDeltaTime)
	{
		m_fCalculDeltaTime = 0.f;
		return true;
	}
	return false;
}

void CFrameMgr::Free()
{
}
