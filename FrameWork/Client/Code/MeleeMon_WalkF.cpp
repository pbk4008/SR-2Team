#include "pch.h"
#include "MeleeMon_WalkF.h"

CMeleeMon_WalkF::CMeleeMon_WalkF()
	: m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CMeleeMon_WalkF::CMeleeMon_WalkF(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice), m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{
}

CMeleeMon_WalkF::CMeleeMon_WalkF(const CMeleeMon_WalkF& rhs)
	:CAnimation(rhs), m_pTransform(rhs.m_pTransform), m_fSpeed(rhs.m_fSpeed), 
	m_fDelayTime(rhs.m_fDelayTime), m_fDirChangeTime(rhs.m_fDirChangeTime)
{
}

CMeleeMon_WalkF::~CMeleeMon_WalkF()
{
}

HRESULT CMeleeMon_WalkF::Init_MeleeMon_WalkF()
{
	m_bLoop = true;
	m_fPlaySpeed = 0.4f;
	m_fSpeed = 1.f;
	InitTexture(L"MeleeMon_WalkF");
	return S_OK;
}

_int CMeleeMon_WalkF::Update_Component(const _float& fDeltaTime)
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

CComponent* CMeleeMon_WalkF::Clone_Component()
{
	return new CMeleeMon_WalkF(*this);
}

void CMeleeMon_WalkF::Setting_Animation(const _float& fDeltaTime)
{

}

void CMeleeMon_WalkF::ResetTimer()
{

}

CMeleeMon_WalkF* CMeleeMon_WalkF::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMeleeMon_WalkF* pInstance = new CMeleeMon_WalkF(pDevice);
	if (FAILED(pInstance->Init_MeleeMon_WalkF()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMeleeMon_WalkF::Free()
{
	Safe_Release(m_pTransform);
	CAnimation::Free();
}

void CMeleeMon_WalkF::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
}
