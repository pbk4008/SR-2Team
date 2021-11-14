#include "pch.h"
#include "Boss_RangeAnim.h"

CBoss_RangeAnim::CBoss_RangeAnim()
	: m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CBoss_RangeAnim::CBoss_RangeAnim(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice), m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CBoss_RangeAnim::CBoss_RangeAnim(const CBoss_RangeAnim& rhs)
	:CAnimation(rhs), m_pTransform(rhs.m_pTransform), m_fSpeed(rhs.m_fSpeed),
	m_fDelayTime(rhs.m_fDelayTime), m_fDirChangeTime(rhs.m_fDirChangeTime)
{

}

CBoss_RangeAnim::~CBoss_RangeAnim()
{

}

HRESULT CBoss_RangeAnim::Init_ShootMon_WalkF()
{
	m_bLoop = true;
	m_fPlaySpeed = 0.28f;
	m_fSpeed = 1.f;
	InitTexture(L"Boss_Range");

	return S_OK;
}

Engine::_int CBoss_RangeAnim::Update_Component(const _float& fDeltaTime)
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

Engine::CComponent* CBoss_RangeAnim::Clone_Component()
{
	return new CBoss_RangeAnim(*this);

}

void CBoss_RangeAnim::Setting_Animation(const _float& fDeltaTime)
{

}

void CBoss_RangeAnim::ResetTimer()
{

}

CBoss_RangeAnim* CBoss_RangeAnim::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBoss_RangeAnim* pInstance = new CBoss_RangeAnim(pDevice);
	if (FAILED(pInstance->Init_ShootMon_WalkF()))
		Safe_Release(pInstance);

	return pInstance;
}

void CBoss_RangeAnim::Free()
{
	Safe_Release(m_pTransform);
	CAnimation::Free();
}

void CBoss_RangeAnim::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
}
