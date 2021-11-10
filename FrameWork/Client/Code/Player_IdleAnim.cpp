#include "pch.h"
#include "Player_IdleAnim.h"

CPlayerIdleAnim::CPlayerIdleAnim()
{
}

CPlayerIdleAnim::CPlayerIdleAnim(LPDIRECT3DDEVICE9 pDevice) : CAnimation(pDevice)
{
}

CPlayerIdleAnim::CPlayerIdleAnim(const CAnimation& rhs) : CAnimation(rhs)
{
}

CPlayerIdleAnim::~CPlayerIdleAnim()
{
}


HRESULT CPlayerIdleAnim::Init_PlayerIdleAnim()
{
	m_bLoop = true;
	m_fPlaySpeed = 0.1f;
	InitTexture(L"PlayerSwordAttack");
	m_iIndex = 0;
	m_bDelay = true;

	return S_OK;
}

_int CPlayerIdleAnim::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CAnimation::Update_Component(fDeltaTime);
	if (iExit & 0x80000000)
		return -1;
	return iExit;
}

CComponent* CPlayerIdleAnim::Clone_Component()
{
	return new CPlayerIdleAnim(*this);
}


CPlayerIdleAnim* CPlayerIdleAnim::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayerIdleAnim* pInstance = new CPlayerIdleAnim(pDevice);
	if (FAILED(pInstance->Init_PlayerIdleAnim()))
		Safe_Release(pInstance);
	return pInstance;
}

void CPlayerIdleAnim::Free()
{
	CAnimation::Free();
}
