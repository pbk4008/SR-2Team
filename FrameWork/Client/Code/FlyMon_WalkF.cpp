#include "pch.h"
#include "FlyMon_WalkF.h"

CFlyMon_WalkF::CFlyMon_WalkF()
	: m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CFlyMon_WalkF::CFlyMon_WalkF(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice), m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{
}

CFlyMon_WalkF::CFlyMon_WalkF(const CFlyMon_WalkF& rhs)
	:CAnimation(rhs), m_pTransform(rhs.m_pTransform), m_fSpeed(rhs.m_fSpeed), 
	m_fDelayTime(rhs.m_fDelayTime), m_fDirChangeTime(rhs.m_fDirChangeTime)
{
}

CFlyMon_WalkF::~CFlyMon_WalkF()
{
}

HRESULT CFlyMon_WalkF::Init_FlyMon_WalkF()
{
	m_bLoop = true;
	m_fPlaySpeed = 0.4f;
	m_fSpeed = 1.f;
	InitTexture(L"FlyMon_WalkF");
	return S_OK;
}

_int CFlyMon_WalkF::Update_Component(const _float& fDeltaTime)
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

CComponent* CFlyMon_WalkF::Clone_Component()
{
	return new CFlyMon_WalkF(*this);
}

void CFlyMon_WalkF::Setting_Animation(const _float& fDeltaTime)
{

}

void CFlyMon_WalkF::ResetTimer()
{

}

CFlyMon_WalkF* CFlyMon_WalkF::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFlyMon_WalkF* pInstance = new CFlyMon_WalkF(pDevice);
	if (FAILED(pInstance->Init_FlyMon_WalkF()))
		Safe_Release(pInstance);

	return pInstance;
}

void CFlyMon_WalkF::Free()
{
	Safe_Release(m_pTransform);
	CAnimation::Free();
}

void CFlyMon_WalkF::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
}
