#include "pch.h"
#include "FlyMon_Idle.h"

CFlyMon_Idle::CFlyMon_Idle()
{

}

CFlyMon_Idle::CFlyMon_Idle(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice)
{

}

CFlyMon_Idle::CFlyMon_Idle(const CAnimation& rhs)
	: CAnimation(rhs)
{

}

CFlyMon_Idle::~CFlyMon_Idle()
{

}

HRESULT CFlyMon_Idle::Init_FlyMon_Idle()
{
	m_bLoop = true;
	m_fPlaySpeed = 0.1f;
	InitTexture(L"FlyMon_Idle");
	m_iIndex = 0;
	m_bDelay = true;

	return S_OK;
}

_int CFlyMon_Idle::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;

	iExit = CAnimation::Update_Component(fDeltaTime);
	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

CComponent* CFlyMon_Idle::Clone_Component()
{
	return new CFlyMon_Idle(*this);
}

CFlyMon_Idle* CFlyMon_Idle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFlyMon_Idle* pInstance = new CFlyMon_Idle(pDevice);
	if (FAILED(pInstance->Init_FlyMon_Idle()))
		Safe_Release(pInstance);
	
	return pInstance;
}

void CFlyMon_Idle::Free()
{
	CAnimation::Free();
}
