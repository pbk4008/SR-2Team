#include "pch.h"
#include "MeleeMon_Attack.h"

CMeleeMon_Attack::CMeleeMon_Attack()
	: m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CMeleeMon_Attack::CMeleeMon_Attack(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice), m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CMeleeMon_Attack::CMeleeMon_Attack(const CMeleeMon_Attack& rhs)
	:CAnimation(rhs), m_pTransform(rhs.m_pTransform), m_fSpeed(rhs.m_fSpeed),
	m_fDelayTime(rhs.m_fDelayTime), m_fDirChangeTime(rhs.m_fDirChangeTime)
{

}

CMeleeMon_Attack::~CMeleeMon_Attack()
{

}

HRESULT CMeleeMon_Attack::Init_MeleeMon_WalkF()
{
	m_bLoop = true;
	m_fPlaySpeed = 0.28f;
	m_fSpeed = 1.f;
	InitTexture(L"MeleeMon_Attack");

	return S_OK;
}

Engine::_int CMeleeMon_Attack::Update_Component(const _float& fDeltaTime)
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

Engine::CComponent* CMeleeMon_Attack::Clone_Component()
{
	return new CMeleeMon_Attack(*this);

}

void CMeleeMon_Attack::Setting_Animation(const _float& fDeltaTime)
{

}

void CMeleeMon_Attack::ResetTimer()
{

}

CMeleeMon_Attack* CMeleeMon_Attack::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMeleeMon_Attack* pInstance = new CMeleeMon_Attack(pDevice);
	if (FAILED(pInstance->Init_MeleeMon_WalkF()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMeleeMon_Attack::Free()
{
	Safe_Release(m_pTransform);
	CAnimation::Free();
}

void CMeleeMon_Attack::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
}
