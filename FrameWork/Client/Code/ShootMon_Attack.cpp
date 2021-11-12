#include "pch.h"
#include "ShootMon_Attack.h"

CShootMon_Attack::CShootMon_Attack()
	: m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CShootMon_Attack::CShootMon_Attack(LPDIRECT3DDEVICE9 pDevice)
	: CAnimation(pDevice), m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f),
	m_fDirChangeTime(0.f)
{

}

CShootMon_Attack::CShootMon_Attack(const CShootMon_Attack& rhs)
	:CAnimation(rhs), m_pTransform(rhs.m_pTransform), m_fSpeed(rhs.m_fSpeed),
	m_fDelayTime(rhs.m_fDelayTime), m_fDirChangeTime(rhs.m_fDirChangeTime)
{

}

CShootMon_Attack::~CShootMon_Attack()
{

}

HRESULT CShootMon_Attack::Init_ShootMon_WalkF()
{
	m_bLoop = true;
	m_fPlaySpeed = 0.28f;
	m_fSpeed = 1.f;
	InitTexture(L"ShootMon_Attack");

	return S_OK;
}

Engine::_int CShootMon_Attack::Update_Component(const _float& fDeltaTime)
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

Engine::CComponent* CShootMon_Attack::Clone_Component()
{
	return new CShootMon_Attack(*this);

}

void CShootMon_Attack::Setting_Animation(const _float& fDeltaTime)
{

}

void CShootMon_Attack::ResetTimer()
{

}

CShootMon_Attack* CShootMon_Attack::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CShootMon_Attack* pInstance = new CShootMon_Attack(pDevice);
	if (FAILED(pInstance->Init_ShootMon_WalkF()))
		Safe_Release(pInstance);

	return pInstance;
}

void CShootMon_Attack::Free()
{
	Safe_Release(m_pTransform);
	CAnimation::Free();
}

void CShootMon_Attack::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
}
