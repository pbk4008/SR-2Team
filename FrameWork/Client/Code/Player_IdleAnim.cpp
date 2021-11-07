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
	return S_OK;
}

_int CPlayerIdleAnim::Update_Component(const _float& fDeltaTime)
{
	return _int();
}

CComponent* CPlayerIdleAnim::Clone_Component()
{
	return new CPlayerIdleAnim(*this);
}

CPlayerIdleAnim* CPlayerIdleAnim::Create(LPDIRECT3DDEVICE9 pDevice, CTexture* pTexture)
{
	CPlayerIdleAnim* pInstance = new CPlayerIdleAnim(pDevice);
	if (FAILED(pInstance->Init_PlayerIdleAnim()))
		Safe_Release(pInstance);
	return nullptr;
}

void CPlayerIdleAnim::Free()
{
	CAnimation::Free();
}
