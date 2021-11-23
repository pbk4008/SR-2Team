#include "pch.h"
#include "MeleeMon_Death.h"

CMeleeMon_Death::CMeleeMon_Death()
	: m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CMeleeMon_Death::CMeleeMon_Death(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice), m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{
}

CMeleeMon_Death::CMeleeMon_Death(const CMeleeMon_Death& rhs)
	:CAnimation(rhs), m_pTransform(rhs.m_pTransform), m_fSpeed(rhs.m_fSpeed), 
	m_fDelayTime(rhs.m_fDelayTime), m_fDirChangeTime(rhs.m_fDirChangeTime)
{
}

CMeleeMon_Death::~CMeleeMon_Death()
{
}

HRESULT CMeleeMon_Death::Init_MeleeMon_Death()
{
	m_bLoop = false;
	m_fPlaySpeed = 0.15f;
	m_fSpeed = 1.f;
	InitTexture(L"MeleeMon_Death");
	return S_OK;
}

_int CMeleeMon_Death::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CAnimation::Update_Component(fDeltaTime);

	if (iExit & 0x80000000)
		return -1;

	return iExit;
}

void CMeleeMon_Death::Render_Animation()
{
	CAnimation::Render_Animation();
}

CComponent* CMeleeMon_Death::Clone_Component()
{
	return new CMeleeMon_Death(*this);
}

CMeleeMon_Death* CMeleeMon_Death::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMeleeMon_Death* pInstance = new CMeleeMon_Death(pDevice);
	if (FAILED(pInstance->Init_MeleeMon_Death()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMeleeMon_Death::Free()
{
	Safe_Release(m_pTransform);
	CAnimation::Free();
}

void CMeleeMon_Death::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
}
