#include "pch.h"
#include "MeleeMon_Idle.h"

CMeleeMon_Idle::CMeleeMon_Idle()
{

}

CMeleeMon_Idle::CMeleeMon_Idle(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice)
{

}

CMeleeMon_Idle::CMeleeMon_Idle(const CAnimation& rhs)
	: CAnimation(rhs)
{

}

CMeleeMon_Idle::~CMeleeMon_Idle()
{

}

HRESULT CMeleeMon_Idle::Init_MeleeMon_Idle()
{
	m_bLoop = true;
	m_fPlaySpeed = 0.1f;
	InitTexture(L"MeleeMon_Idle");
	m_iIndex = 0;
	m_bDelay = true;

	return S_OK;
}

_int CMeleeMon_Idle::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;

	iExit = CAnimation::Update_Component(fDeltaTime);
	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

CComponent* CMeleeMon_Idle::Clone_Component()
{
	return new CMeleeMon_Idle(*this);
}

CMeleeMon_Idle* CMeleeMon_Idle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMeleeMon_Idle* pInstance = new CMeleeMon_Idle(pDevice);
	if (FAILED(pInstance->Init_MeleeMon_Idle()))
		Safe_Release(pInstance);
	
	return pInstance;
}

void CMeleeMon_Idle::Free()
{
	CAnimation::Free();
}
