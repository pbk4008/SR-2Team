#include "pch.h"
#include "Boss_ChargeAnim.h"

CBoss_ChargeAnim::CBoss_ChargeAnim()
	: m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CBoss_ChargeAnim::CBoss_ChargeAnim(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice), m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{
}

CBoss_ChargeAnim::CBoss_ChargeAnim(const CBoss_ChargeAnim& rhs)
	:CAnimation(rhs), m_pTransform(rhs.m_pTransform), m_fSpeed(rhs.m_fSpeed), 
	m_fDelayTime(rhs.m_fDelayTime), m_fDirChangeTime(rhs.m_fDirChangeTime)
{
}

CBoss_ChargeAnim::~CBoss_ChargeAnim()
{
}

HRESULT CBoss_ChargeAnim::Init_Boss_ChargeAnim()
{
	m_bLoop = true;
	m_fPlaySpeed = 0.4f;
	m_fSpeed = 1.f;
	InitTexture(L"Boss_Charge");
	return S_OK;
}

_int CBoss_ChargeAnim::Update_Component(const _float& fDeltaTime)
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

CComponent* CBoss_ChargeAnim::Clone_Component()
{
	return new CBoss_ChargeAnim(*this);
}

void CBoss_ChargeAnim::Setting_Animation(const _float& fDeltaTime)
{

}

void CBoss_ChargeAnim::ResetTimer()
{

}

CBoss_ChargeAnim* CBoss_ChargeAnim::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBoss_ChargeAnim* pInstance = new CBoss_ChargeAnim(pDevice);
	if (FAILED(pInstance->Init_Boss_ChargeAnim()))
		Safe_Release(pInstance);

	return pInstance;
}

void CBoss_ChargeAnim::Free()
{
	Safe_Release(m_pTransform);
	CAnimation::Free();
}

void CBoss_ChargeAnim::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
}
