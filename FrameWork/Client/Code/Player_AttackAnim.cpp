#include "pch.h"
#include "Player_AttackAnim.h"


CPlayer_AttackAnim::CPlayer_AttackAnim() : m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f)
, m_fDirChangeTime(0.f)
{
}

CPlayer_AttackAnim::CPlayer_AttackAnim(LPDIRECT3DDEVICE9 pDevice) : CAnimation(pDevice), m_pTransform(nullptr), m_fSpeed(0.f), m_fDelayTime(0.f)
, m_fDirChangeTime(0.f)
{
}

CPlayer_AttackAnim::CPlayer_AttackAnim(const CPlayer_AttackAnim& rhs) : CAnimation(rhs),m_pTransform(rhs.m_pTransform), m_fSpeed(rhs.m_fSpeed), m_fDelayTime(rhs.m_fDelayTime)
, m_fDirChangeTime(rhs.m_fDirChangeTime)
{
	if (rhs.m_pTransform)
		m_pTransform->AddRef();
}

CPlayer_AttackAnim::~CPlayer_AttackAnim()
{
}

HRESULT CPlayer_AttackAnim::Init_PlayerAttackAnim()
{
	m_bLoop = false;
	m_fPlaySpeed = 0.1f;
	m_fSpeed = 1.f;
	InitTexture(L"PlayerSwordAttack");

	return S_OK;
}

_int CPlayer_AttackAnim::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CAnimation::Update_Component(fDeltaTime);
	if (iExit & 0x80000000)
		return -1;
	SettingAnimation(fDeltaTime);
	if (!m_bPlay)
		ResetTimer();
	return iExit;
}

CComponent* CPlayer_AttackAnim::Clone_Component()
{
	return new CPlayer_AttackAnim(*this);
}

void CPlayer_AttackAnim::LeftMove(const _float& fDeltaTime)
{
	_vec3 vPos = m_pTransform->getPos();;
	_vec3 vDir = _vec3(-1.f,0.f,0.f);
	vPos += vDir * m_fSpeed * fDeltaTime;

	m_pTransform->setPos(vPos);
}

void CPlayer_AttackAnim::UpMove(const _float& fDeltaTime)
{
	_vec3 vPos = m_pTransform->getPos();
 	_vec3 vDir = _vec3(-1.f, 1.f, 0.f);
	D3DXVec3Normalize(&vDir, &vDir);
	vPos += vDir * m_fSpeed * fDeltaTime;

	m_pTransform->setPos(vPos);
} 

void CPlayer_AttackAnim::DownMove(const _float& fDeltaTime)
{ 
	_vec3 vPos = m_pTransform->getPos();
  	_vec3 vDir = _vec3(-1.f, -0.5f, 0.f);
	D3DXVec3Normalize(&vDir, &vDir);
	vPos += vDir * m_fSpeed * fDeltaTime;

	m_pTransform->setPos(vPos);
}

void CPlayer_AttackAnim::SettingAnimation(const _float& fDeltaTime)
{
	if (m_iIndex < 2)
	{
		m_fSpeed = 3.f;
		UpMove(fDeltaTime);
	}
	else
	{
		//m_fDirChangeTime += fDeltaTime;
		//if (m_fDirChangeTime > 0.45f)
		//	DownMove(fDeltaTime);
		//else
		//	LeftMove(fDeltaTime);
		DownMove(fDeltaTime);
	}
	if (m_iIndex == 3)
	{
		m_fSpeed = 7.f;
		m_bDelay = true;
		m_fDelayTime += fDeltaTime;
		if (m_fDelayTime > 0.5f)
		{
			m_bDelay = false;
			m_fDelayTime = 0.f;
		}
	}
	
}

CPlayer_AttackAnim* CPlayer_AttackAnim::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayer_AttackAnim* pInstance = new CPlayer_AttackAnim(pDevice);
	if (FAILED(pInstance->Init_PlayerAttackAnim()))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_AttackAnim::Free()
{
	Safe_Release(m_pTransform);
	CAnimation::Free();
}

void CPlayer_AttackAnim::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
	
}

void CPlayer_AttackAnim::ResetTimer()
{
	m_fDelayTime = 0.f;
	m_fDirChangeTime = 0.f;
	m_fSpeed = 1.f;
	m_bDelay = false;
	m_pTransform->setPos(-0.15f, 0.f, 0.f);
}
