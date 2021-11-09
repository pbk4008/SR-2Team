#include "pch.h"
#include "Player_Walk.h"

CPlayerWalk::CPlayerWalk() : m_pTransform(nullptr), m_fMoveTime(0.f), m_fSpeed(0.f)
{
}

CPlayerWalk::CPlayerWalk(LPDIRECT3DDEVICE9 pDevice) : CAnimation(pDevice), m_pTransform(nullptr)
,m_fMoveTime(0.f), m_fSpeed(0.f)
{
}

CPlayerWalk::CPlayerWalk(const CPlayerWalk& rhs) : CAnimation(rhs), m_pTransform(rhs.m_pTransform)
,m_fMoveTime(rhs.m_fMoveTime), m_fSpeed(rhs.m_fSpeed)
{
	if (rhs.m_pTransform)
		m_pTransform->AddRef();
}

CPlayerWalk::~CPlayerWalk()
{
}

HRESULT CPlayerWalk::Init_PlayerWalk(CTexture* pTexture)
{
	m_bLoop = true;
	m_fPlaySpeed = 0.1f;
	setTexture(pTexture);
	m_iIndex = 0;
	m_bDelay = true;
	m_fSpeed = 0.3f;

	return S_OK;
}

_int CPlayerWalk::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;

	iExit = CAnimation::Update_Component(fDeltaTime);
	SettingAnimation(fDeltaTime);
	return iExit;
}


CComponent* CPlayerWalk::Clone_Component()
{
	return new CPlayerWalk(*this);
}
void CPlayerWalk::SettingAnimation(const _float& fDeltaTime)
{
	Move(fDeltaTime);
}
void CPlayerWalk::Move(const float& fDeltaTime)
{
	m_fMoveTime += fDeltaTime;
	_vec3 vPos = m_pTransform->getPos();
	_vec3 vDir = *m_pTransform->getAxis(VECAXIS::AXIS_RIGHT);
	D3DXVec3Normalize(&vDir, &vDir);

	if (m_fMoveTime > 0.5f)
	{
		m_bLeftRightCheck = !m_bLeftRightCheck;
		m_fMoveTime = 0.f;
	}

	if(m_bLeftRightCheck)
		vPos +=vDir* m_fSpeed* fDeltaTime;
	else
		vPos +=vDir* -m_fSpeed* fDeltaTime;

	m_pTransform->setPos(vPos);
}

void CPlayerWalk::Free()
{
	Safe_Release(m_pTransform);
	CAnimation::Free();
}

void CPlayerWalk::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
}

CPlayerWalk* CPlayerWalk::Create(LPDIRECT3DDEVICE9 pDevice, CTexture* pTexture)
{
	CPlayerWalk* pInstance = new CPlayerWalk(pDevice);
	if (FAILED(pInstance->Init_PlayerWalk(pTexture)))
		Safe_Release(pInstance);
	return pInstance;
}
