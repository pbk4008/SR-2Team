#include "pch.h"
#include "Boss_DeathAnim.h"

CBoss_DeathAnim::CBoss_DeathAnim()
	: m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CBoss_DeathAnim::CBoss_DeathAnim(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice), m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{
}

CBoss_DeathAnim::CBoss_DeathAnim(const CBoss_DeathAnim& rhs)
	:CAnimation(rhs), m_pTransform(rhs.m_pTransform), m_fSpeed(rhs.m_fSpeed), 
	m_fDelayTime(rhs.m_fDelayTime), m_fDirChangeTime(rhs.m_fDirChangeTime)
{
}

CBoss_DeathAnim::~CBoss_DeathAnim()
{
}

HRESULT CBoss_DeathAnim::Init_Boss_DeathAnim()
{
	m_bLoop = false;
	m_fPlaySpeed = 0.15f;
	m_fSpeed = 1.f;
	InitTexture(L"Boss_Death");
	return S_OK;
}

_int CBoss_DeathAnim::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CAnimation::Update_Component(fDeltaTime);

	if (iExit & 0x80000000)
		return -1;

	Setting_Animation(fDeltaTime);
	if (!m_bPlay)
		ResetTimer();

	return iExit;
}

CComponent* CBoss_DeathAnim::Clone_Component()
{
	return new CBoss_DeathAnim(*this);
}

void CBoss_DeathAnim::Setting_Animation(const _float& fDeltaTime)
{

}

void CBoss_DeathAnim::ResetTimer()
{

}

CBoss_DeathAnim* CBoss_DeathAnim::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBoss_DeathAnim* pInstance = new CBoss_DeathAnim(pDevice);
	if (FAILED(pInstance->Init_Boss_DeathAnim()))
		Safe_Release(pInstance);

	return pInstance;
}

void CBoss_DeathAnim::Free()
{
	Safe_Release(m_pTransform);
	CAnimation::Free();
}

void CBoss_DeathAnim::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
}
