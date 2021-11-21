#include "pch.h"
#include "Boss_IdleAnim.h"

CBoss_IdleAnim::CBoss_IdleAnim()
{

}

CBoss_IdleAnim::CBoss_IdleAnim(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice)
{

}

CBoss_IdleAnim::CBoss_IdleAnim(const CAnimation& rhs)
	: CAnimation(rhs)
{

}

CBoss_IdleAnim::~CBoss_IdleAnim()
{

}

HRESULT CBoss_IdleAnim::Init_Boss_IdleAnim()
{
	m_bLoop = true;
	m_fPlaySpeed = 0.1f;
	InitTexture(L"Boss_Idle");
	m_iIndex = 0;
	m_bDelay = true;

	return S_OK;
}

_int CBoss_IdleAnim::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;

	iExit = CAnimation::Update_Component(fDeltaTime);
	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

CComponent* CBoss_IdleAnim::Clone_Component()
{
	return new CBoss_IdleAnim(*this);
}

CBoss_IdleAnim* CBoss_IdleAnim::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBoss_IdleAnim* pInstance = new CBoss_IdleAnim(pDevice);
	if (FAILED(pInstance->Init_Boss_IdleAnim()))
		Safe_Release(pInstance);
	
	return pInstance;
}

void CBoss_IdleAnim::Free()
{
	CAnimation::Free();
}
