#include "pch.h"
#include "FlyMon_Attack.h"

CFlyMon_Attack::CFlyMon_Attack()
	: m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CFlyMon_Attack::CFlyMon_Attack(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice), m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CFlyMon_Attack::CFlyMon_Attack(const CFlyMon_Attack& rhs)
	:CAnimation(rhs), m_pTransform(rhs.m_pTransform), m_fSpeed(rhs.m_fSpeed),
	m_fDelayTime(rhs.m_fDelayTime), m_fDirChangeTime(rhs.m_fDirChangeTime)
{

}

CFlyMon_Attack::~CFlyMon_Attack()
{

}

HRESULT CFlyMon_Attack::Init_FlyMon_WalkF()
{
	m_bLoop = true;
	m_fPlaySpeed = 0.28f;
	m_fSpeed = 1.f;
	InitTexture(L"FlyMon_Attack");

	return S_OK;
}

Engine::_int CFlyMon_Attack::Update_Component(const _float& fDeltaTime)
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

Engine::CComponent* CFlyMon_Attack::Clone_Component()
{
	return new CFlyMon_Attack(*this);

}

void CFlyMon_Attack::Setting_Animation(const _float& fDeltaTime)
{

}

void CFlyMon_Attack::ResetTimer()
{

}

CFlyMon_Attack* CFlyMon_Attack::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFlyMon_Attack* pInstance = new CFlyMon_Attack(pDevice);
	if (FAILED(pInstance->Init_FlyMon_WalkF()))
		Safe_Release(pInstance);

	return pInstance;
}

void CFlyMon_Attack::Free()
{
	Safe_Release(m_pTransform);
	CAnimation::Free();
}

void CFlyMon_Attack::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
}
