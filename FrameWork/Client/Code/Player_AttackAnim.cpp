#include "pch.h"
#include "Player_AttackAnim.h"

CPlayer_AttackAnim::CPlayer_AttackAnim()
{
}

CPlayer_AttackAnim::CPlayer_AttackAnim(LPDIRECT3DDEVICE9 pDevice) : CAnimation(pDevice)
{
}

CPlayer_AttackAnim::CPlayer_AttackAnim(const CPlayer_AttackAnim& rhs) : CAnimation(rhs)
{
}

CPlayer_AttackAnim::~CPlayer_AttackAnim()
{
}

HRESULT CPlayer_AttackAnim::Init_PlayerAttackAnim(CTexture* pTexture)
{
	m_bLoop = false;
	m_fPlaySpeed = 0.5f;
	setTexture(pTexture);

	return S_OK;
}

_int CPlayer_AttackAnim::Update_Component(const _float& fDeltaTime)
{
	_int iExit = CAnimation::Update_Component(fDeltaTime);
	return iExit;
}

CComponent* CPlayer_AttackAnim::Clone_Component()
{
	return new CPlayer_AttackAnim(*this);
}

CPlayer_AttackAnim* CPlayer_AttackAnim::Create(LPDIRECT3DDEVICE9 pDevice, CTexture* pTexture)
{
	CPlayer_AttackAnim* pInstance = new CPlayer_AttackAnim(pDevice);
	if (FAILED(pInstance->Init_PlayerAttackAnim(pTexture)))
		Safe_Release(pInstance);
	return pInstance;
}

void CPlayer_AttackAnim::Free()
{
	CAnimation::Free();
}
