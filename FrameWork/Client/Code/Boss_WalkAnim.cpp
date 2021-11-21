#include "pch.h"
#include "Boss_WalkAnim.h"

CBoss_WalkAnim::CBoss_WalkAnim()
	: m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CBoss_WalkAnim::CBoss_WalkAnim(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice), m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{
}

CBoss_WalkAnim::CBoss_WalkAnim(const CBoss_WalkAnim& rhs)
	:CAnimation(rhs), m_pTransform(rhs.m_pTransform), m_fSpeed(rhs.m_fSpeed), 
	m_fDelayTime(rhs.m_fDelayTime), m_fDirChangeTime(rhs.m_fDirChangeTime)
{
}

CBoss_WalkAnim::~CBoss_WalkAnim()
{
}

HRESULT CBoss_WalkAnim::Init_Boss_WalkAnim()
{
	m_bLoop = true;
	m_fPlaySpeed = 0.4f;
	m_fSpeed = 1.f;
	InitTexture(L"Boss_Move");
	return S_OK;
}

_int CBoss_WalkAnim::Update_Component(const _float& fDeltaTime)
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

CComponent* CBoss_WalkAnim::Clone_Component()
{
	return new CBoss_WalkAnim(*this);
}

void CBoss_WalkAnim::Setting_Animation(const _float& fDeltaTime)
{

}

void CBoss_WalkAnim::ResetTimer()
{

}

CBoss_WalkAnim* CBoss_WalkAnim::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBoss_WalkAnim* pInstance = new CBoss_WalkAnim(pDevice);
	if (FAILED(pInstance->Init_Boss_WalkAnim()))
		Safe_Release(pInstance);

	return pInstance;
}

void CBoss_WalkAnim::Free()
{
	Safe_Release(m_pTransform);
	CAnimation::Free();
}

void CBoss_WalkAnim::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
}
