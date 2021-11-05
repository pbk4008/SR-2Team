#include "Engine_Include.h"
#include "FrameMgr.h"
#include <iostream>
using namespace std;
IMPLEMENT_SINGLETON(CFrameMgr)
CFrameMgr::CFrameMgr(): m_fCalculDeltaTime(0.f), m_fLimitFrame(0.f), m_iFPS(0), m_fFPSTime(0), m_fOutDeltaTime(0.f)
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
	m_fFPSTime += fDeltaTime;
	if(m_fFPSTime > 1.f)//1초동안하는
	{
		cout << m_iFPS << endl;
		m_fFPSTime = 0.f;
		m_iFPS = 0;
	}
	if (m_fLimitFrame <= m_fCalculDeltaTime)//1Frame에서 해야할 일
	{
		m_iFPS++;
		m_fOutDeltaTime = m_fCalculDeltaTime;
		m_fCalculDeltaTime = 0.f;
		return true;
	}
	
	return false;
}

void CFrameMgr::Free()
{
}
