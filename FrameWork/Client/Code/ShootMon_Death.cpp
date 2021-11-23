#include "pch.h"
#include "ShootMon_Death.h"

CShootMon_Death::CShootMon_Death()
	: m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CShootMon_Death::CShootMon_Death(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice), m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{
}

CShootMon_Death::CShootMon_Death(const CShootMon_Death& rhs)
	:CAnimation(rhs), m_pTransform(rhs.m_pTransform), m_fSpeed(rhs.m_fSpeed), 
	m_fDelayTime(rhs.m_fDelayTime), m_fDirChangeTime(rhs.m_fDirChangeTime)
{
}

CShootMon_Death::~CShootMon_Death()
{
}

HRESULT CShootMon_Death::Init_ShootMon_Death()
{
	m_bLoop = false;
	m_fPlaySpeed = 0.15f;
	m_fSpeed = 1.f;
	InitTexture(L"ShootMon_Death");
	return S_OK;
}

_int CShootMon_Death::Update_Component(const _float& fDeltaTime)
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

void CShootMon_Death::Render_Animation()
{
	CAnimation::Render_Animation();
}

CComponent* CShootMon_Death::Clone_Component()
{
	return new CShootMon_Death(*this);
}

void CShootMon_Death::Setting_Animation(const _float& fDeltaTime)
{

}

void CShootMon_Death::ResetTimer()
{

}

CShootMon_Death* CShootMon_Death::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CShootMon_Death* pInstance = new CShootMon_Death(pDevice);
	if (FAILED(pInstance->Init_ShootMon_Death()))
		Safe_Release(pInstance);

	return pInstance;
}

void CShootMon_Death::Free()
{
	Safe_Release(m_pTransform);
	CAnimation::Free();
}

void CShootMon_Death::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
}
