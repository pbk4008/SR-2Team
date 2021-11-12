#include "pch.h"
#include "ShootMon_WalkF.h"

CShootMon_WalkF::CShootMon_WalkF()
	: m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CShootMon_WalkF::CShootMon_WalkF(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice), m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{
}

CShootMon_WalkF::CShootMon_WalkF(const CShootMon_WalkF& rhs)
	:CAnimation(rhs), m_pTransform(rhs.m_pTransform), m_fSpeed(rhs.m_fSpeed), 
	m_fDelayTime(rhs.m_fDelayTime), m_fDirChangeTime(rhs.m_fDirChangeTime)
{
}

CShootMon_WalkF::~CShootMon_WalkF()
{
}

HRESULT CShootMon_WalkF::Init_ShootMon_WalkF()
{
	m_bLoop = true;
	m_fPlaySpeed = 0.4f;
	m_fSpeed = 1.f;
	InitTexture(L"ShootMon_WalkF");
	return S_OK;
}

_int CShootMon_WalkF::Update_Component(const _float& fDeltaTime)
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

CComponent* CShootMon_WalkF::Clone_Component()
{
	return new CShootMon_WalkF(*this);
}

void CShootMon_WalkF::Setting_Animation(const _float& fDeltaTime)
{

}

void CShootMon_WalkF::ResetTimer()
{

}

CShootMon_WalkF* CShootMon_WalkF::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CShootMon_WalkF* pInstance = new CShootMon_WalkF(pDevice);
	if (FAILED(pInstance->Init_ShootMon_WalkF()))
		Safe_Release(pInstance);

	return pInstance;
}

void CShootMon_WalkF::Free()
{
	Safe_Release(m_pTransform);
	CAnimation::Free();
}

void CShootMon_WalkF::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
}
