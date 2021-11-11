#include "pch.h"
#include "Player_AttackAnim.h"
#include "Player.h"

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
	TypeBySetAnimation(fDeltaTime);

	if (!m_bPlay)
		ResetTimer();
	return iExit;
}

CComponent* CPlayer_AttackAnim::Clone_Component()
{
	return new CPlayer_AttackAnim(*this);
}

void CPlayer_AttackAnim::Render_Animation()
{
	m_pDevice->SetTransform(D3DTS_WORLD,&m_pTransform->getWorldMatrix());
	CAnimation::Render_Animation();
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
}

void CPlayer_AttackAnim::TypeBySetAnimation(const _float& fDeltaTime)
{
	CPlayer* pPlayer = static_cast<CPlayer*>(GetGameObject(LAYERID::GAME_LOGIC,GAMEOBJECTID::PLAYER));
	switch (pPlayer->getAttackType())
	{
	case CPlayer::ATTACKTYPE::SWORD:
		SwordSettingAnimation(fDeltaTime);
		break;
	case CPlayer::ATTACKTYPE::SHURIKEN:
		ShurikenSettingAnimation(fDeltaTime);
		break;
	case CPlayer::ATTACKTYPE::GUN:
		GunSettingAnimation(fDeltaTime);
		break;
	}
	m_pTransform->Update_Component(fDeltaTime);
}

void CPlayer_AttackAnim::SwordSettingAnimation(const _float& fDeltaTime)
{
	m_fPlaySpeed = 0.05f;
	if (m_iIndex < 2)
	{
		m_fSpeed = 4.f;
		UpMove(fDeltaTime);
	}
	else
	{
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

void CPlayer_AttackAnim::ShurikenSettingAnimation(const _float& fDeltaTime)
{
	m_fPlaySpeed = 0.03f;
	if (m_iIndex == 4)
	{
		m_bDelay = true;
		m_fDelayTime += fDeltaTime;
		if (m_fDelayTime > 0.15)
		{
			m_fDelayTime = 0.f;
			m_bDelay = false;
		}
	}
}

void CPlayer_AttackAnim::GunSettingAnimation(const _float& fDeltaTime)
{
}
