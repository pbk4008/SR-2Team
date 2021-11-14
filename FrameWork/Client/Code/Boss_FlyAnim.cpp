#include "pch.h"
#include "Boss_FlyAnim.h"

CBoss_FlyAnim::CBoss_FlyAnim()
	: m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CBoss_FlyAnim::CBoss_FlyAnim(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice), m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{
}

CBoss_FlyAnim::CBoss_FlyAnim(const CBoss_FlyAnim& rhs)
	:CAnimation(rhs), m_pTransform(rhs.m_pTransform), m_fSpeed(rhs.m_fSpeed), 
	m_fDelayTime(rhs.m_fDelayTime), m_fDirChangeTime(rhs.m_fDirChangeTime)
{
}

CBoss_FlyAnim::~CBoss_FlyAnim()
{
}

HRESULT CBoss_FlyAnim::Init_Boss_FlyAnim()
{
	m_bLoop = true;
	m_fPlaySpeed = 0.4f;
	m_fSpeed = 1.f;
	InitTexture(L"Boss_FlyAnim");
	return S_OK;
}

_int CBoss_FlyAnim::Update_Component(const _float& fDeltaTime)
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

CComponent* CBoss_FlyAnim::Clone_Component()
{
	return new CBoss_FlyAnim(*this);
}

void CBoss_FlyAnim::Setting_Animation(const _float& fDeltaTime)
{

}

void CBoss_FlyAnim::ResetTimer()
{

}

CBoss_FlyAnim* CBoss_FlyAnim::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBoss_FlyAnim* pInstance = new CBoss_FlyAnim(pDevice);
	if (FAILED(pInstance->Init_Boss_FlyAnim()))
		Safe_Release(pInstance);

	return pInstance;
}

void CBoss_FlyAnim::Free()
{
	Safe_Release(m_pTransform);
	CAnimation::Free();
}

void CBoss_FlyAnim::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
}
