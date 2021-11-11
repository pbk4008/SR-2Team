#include "pch.h"
#include "FlyMon_Death.h"

CFlyMon_Death::CFlyMon_Death()
	: m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CFlyMon_Death::CFlyMon_Death(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice), m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{
}

CFlyMon_Death::CFlyMon_Death(const CFlyMon_Death& rhs)
	:CAnimation(rhs), m_pTransform(rhs.m_pTransform), m_fSpeed(rhs.m_fSpeed), 
	m_fDelayTime(rhs.m_fDelayTime), m_fDirChangeTime(rhs.m_fDirChangeTime)
{
}

CFlyMon_Death::~CFlyMon_Death()
{
}

HRESULT CFlyMon_Death::Init_FlyMon_Death()
{
	m_bLoop = false;
	m_fPlaySpeed = 0.15f;
	m_fSpeed = 1.f;
	InitTexture(L"FlyMon_Death");
	return S_OK;
}

_int CFlyMon_Death::Update_Component(const _float& fDeltaTime)
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

CComponent* CFlyMon_Death::Clone_Component()
{
	return new CFlyMon_Death(*this);
}

void CFlyMon_Death::Setting_Animation(const _float& fDeltaTime)
{

}

void CFlyMon_Death::ResetTimer()
{

}

CFlyMon_Death* CFlyMon_Death::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFlyMon_Death* pInstance = new CFlyMon_Death(pDevice);
	if (FAILED(pInstance->Init_FlyMon_Death()))
		Safe_Release(pInstance);

	return pInstance;
}

void CFlyMon_Death::Free()
{
	Safe_Release(m_pTransform);
	CAnimation::Free();
}

void CFlyMon_Death::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
}
