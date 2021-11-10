#include "pch.h"
#include "ShootMon_Idle.h"

CShootMon_Idle::CShootMon_Idle()
{

}

CShootMon_Idle::CShootMon_Idle(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice)
{

}

CShootMon_Idle::CShootMon_Idle(const CAnimation& rhs)
	: CAnimation(rhs)
{

}

CShootMon_Idle::~CShootMon_Idle()
{

}

HRESULT CShootMon_Idle::Init_ShootMon_Idle()
{
	m_bLoop = true;
	m_fPlaySpeed = 0.1f;
	InitTexture(L"ShootMon_Idle");
	m_iIndex = 0;
	m_bDelay = true;

	return S_OK;
}

_int CShootMon_Idle::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;

	iExit = CAnimation::Update_Component(fDeltaTime);
	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

CComponent* CShootMon_Idle::Clone_Component()
{
	return new CShootMon_Idle(*this);
}

CShootMon_Idle* CShootMon_Idle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CShootMon_Idle* pInstance = new CShootMon_Idle(pDevice);
	if (FAILED(pInstance->Init_ShootMon_Idle()))
		Safe_Release(pInstance);
	
	return pInstance;
}

void CShootMon_Idle::Free()
{
	CAnimation::Free();
}
