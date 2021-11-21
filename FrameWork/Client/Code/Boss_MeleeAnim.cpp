#include "pch.h"
#include "Boss_MeleeAnim.h"

CBoss_MeleeAnim::CBoss_MeleeAnim()
	: m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CBoss_MeleeAnim::CBoss_MeleeAnim(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice), m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CBoss_MeleeAnim::CBoss_MeleeAnim(const CBoss_MeleeAnim& rhs)
	:CAnimation(rhs), m_pTransform(rhs.m_pTransform), m_fSpeed(rhs.m_fSpeed),
	m_fDelayTime(rhs.m_fDelayTime), m_fDirChangeTime(rhs.m_fDirChangeTime)
{

}

CBoss_MeleeAnim::~CBoss_MeleeAnim()
{

}

HRESULT CBoss_MeleeAnim::Init_MeleeMon_WalkF()
{
	m_bLoop = true;
	m_fPlaySpeed = 0.28f;
	m_fSpeed = 1.f;
	InitTexture(L"Boss_Melee");

	return S_OK;
}

Engine::_int CBoss_MeleeAnim::Update_Component(const _float& fDeltaTime)
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

Engine::CComponent* CBoss_MeleeAnim::Clone_Component()
{
	return new CBoss_MeleeAnim(*this);

}

void CBoss_MeleeAnim::Setting_Animation(const _float& fDeltaTime)
{

}

void CBoss_MeleeAnim::ResetTimer()
{

}

CBoss_MeleeAnim* CBoss_MeleeAnim::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBoss_MeleeAnim* pInstance = new CBoss_MeleeAnim(pDevice);
	if (FAILED(pInstance->Init_MeleeMon_WalkF()))
		Safe_Release(pInstance);

	return pInstance;
}

void CBoss_MeleeAnim::Free()
{
	Safe_Release(m_pTransform);
	CAnimation::Free();
}

void CBoss_MeleeAnim::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
}
