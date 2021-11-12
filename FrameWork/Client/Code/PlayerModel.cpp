#include "pch.h"
#include "PlayerModel.h"
#include "Animator.h"
#include "Player_AttackAnim.h"
#include "Player_IdleAnim.h"
#include "Player_Walk.h"
#include "Collision.h"

CPlayerModel::CPlayerModel() : m_pBufferCom(nullptr), m_eState(CPlayer::STATE::MAX), m_pAnimator(nullptr)
, m_eAttackType(CPlayer::ATTACKTYPE::SWORD), m_fChangeTime(0.f), m_bChange(false)
{
}

CPlayerModel::CPlayerModel(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pBufferCom(nullptr), m_pAnimator(nullptr), m_eState(CPlayer::STATE::MAX)
, m_eAttackType(CPlayer::ATTACKTYPE::SWORD), m_fChangeTime(0.f), m_bChange(false)
{
}

CPlayerModel::CPlayerModel(const CPlayerModel& rhs) : CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pAnimator(rhs.m_pAnimator), m_eState(rhs.m_eState)
, m_eAttackType(rhs.m_eAttackType), m_fChangeTime(rhs.m_fChangeTime), m_bChange(rhs.m_bChange)
{
	m_pBufferCom->AddRef();
	if(rhs.m_pAnimator)
		m_pAnimator->AddRef();
	
}

CPlayerModel::~CPlayerModel()
{
}

HRESULT CPlayerModel::Init_PlayerModel()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eState = CPlayer::STATE::IDLE;
	m_eAttackType = CPlayer::ATTACKTYPE::SWORD;
	return S_OK;
}

_int CPlayerModel::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;

	Changing(fDeltaTime);
	iExit = CGameObject::Update_GameObject(fDeltaTime);
	return iExit;
}

void CPlayerModel::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CPlayerModel::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	m_pAnimator->Render_Animator();
	m_pBufferCom->Render_Buffer();
}

HRESULT CPlayerModel::Add_Component()
{
	CGameObject::Add_Component();

	CComponent* pCom = nullptr;

	pCom = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBufferCom->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pCom);

	
	return S_OK;
}

void CPlayerModel::Free()
{
	Safe_Release(m_pAnimator);
	CGameObject::Free();
	Safe_Release(m_pBufferCom);
}

void CPlayerModel::setTarget(CTransform* pTarget)
{
	m_pTransform->setParent(pTarget);
	m_pTransform->setPos(-0.15f, 0.f, 0.f);
}

void CPlayerModel::setState(CPlayer::STATE eState)
{
	m_eState = eState;
}

void CPlayerModel::setAttackType(CPlayer::ATTACKTYPE eType)
{
	m_bChange = true;
	m_eAttackType = eType;
}

CGameObject* CPlayerModel::Clone_GameObject()
{
	return new CPlayerModel(*this);
}

HRESULT CPlayerModel::SettingAnimator()
{
	m_pAnimator = Clone_ComProto<CAnimator>(COMPONENTID::ANIMATOR);

	CAnimation* pAnim = CPlayerIdleAnim::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"Player_Idle", L"Head", pAnim);

	CPlayer_AttackAnim* pAtk = CPlayer_AttackAnim::Create(m_pDevice);
	pAtk->setTransform(m_pTransform);
	pAnim = pAtk;
	m_pAnimator->Insert_Animation(L"Player_Attack", L"Player_Idle", pAnim, true);

	CPlayerWalk* pWalk = CPlayerWalk::Create(m_pDevice);
	pWalk->setTransform(m_pTransform);
	pAnim = pWalk;
	m_pAnimator->Insert_Animation(L"Player_Walk", L"Player_Idle", pAnim, true);

	m_pAnimator->Connet_Animation(L"Player_Attack", L"Player_Walk"); 

	FAILED_CHECK(m_pAnimator->Change_Animation(L"Player_Idle"));

	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::ANIMATOR, m_pAnimator);

	return S_OK;
}

CPlayer::STATE CPlayerModel::Act()
{
	switch (m_eState)
	{
	case CPlayer::STATE::IDLE:
		m_bAttack = false;
		m_pAnimator->Change_Animation(L"Player_Idle");
		break;
	case CPlayer::STATE::ATTACK:
		if (!lstrcmp(m_pAnimator->getCurrentAnim(), L"Player_Attack"))
		{
			if (!m_pAnimator->getAnimPlay())
				m_eState = CPlayer::STATE::IDLE;
		}
		else
		{
			m_pAnimator->Change_Animation(L"Player_Attack");
			m_bAttack = true;
		}
		break;
	case CPlayer::STATE::WALK:
		if (!m_pAnimator->getAnimPlay())
		{
			m_pAnimator->Change_Animation(L"Player_Walk");
			m_bAttack = false;
		}
		break;
	}
	return m_eState;
}

void CPlayerModel::Change_AnimTexture()
{
	switch (m_eAttackType)
	{
	case CPlayer::ATTACKTYPE::SWORD:
		m_pAnimator->Change_AnimationTexture(L"PlayerSwordAttack");
		break;
	case CPlayer::ATTACKTYPE::SHURIKEN:
		m_pAnimator->Change_AnimationTexture(L"PlayerShurikenAttack");
		break;
	case CPlayer::ATTACKTYPE::BOMB:
		m_pAnimator->Change_AnimationTexture(L"PlayerBombAttack");
		break;
	}
}

void CPlayerModel::Changing(const _float& fDeltaTime)
{
	if (m_bChange)
	{
		m_fChangeTime += GetOutDeltaTime();
		_vec3 vDown;
		m_pTransform->getAxis(VECAXIS::AXIS_UP, vDown);
		_vec3 vPos = m_pTransform->getPos();
		D3DXVec3Normalize(&vDown, &vDown);
		if (m_fChangeTime < 0.6f)
			vDown *= -1;
		vPos += vDown * 5.5f * fDeltaTime;
		m_pTransform->setPos(vPos);
		if(m_fChangeTime > 1.f)
		{
			Change_AnimTexture();
			m_fChangeTime = 0.f;
			m_bChange = false;
		}
	}
	else
	{
		if (!m_bAttack)
		{
			switch (m_eAttackType)
			{
			case CPlayer::ATTACKTYPE::SWORD:
				m_pTransform->setPos(-0.15f, 0.f, 0.f);
				m_pTransform->setScale(1.f, 0.8f, 0.5f);
				break;
			case CPlayer::ATTACKTYPE::SHURIKEN:
				m_pTransform->setPos(-0.15f, -0.3f, 0.f);
				m_pTransform->setScale(1.f, 0.5f, 0.5f);
				break;
			case CPlayer::ATTACKTYPE::BOMB:
				m_pTransform->setPos(-0.15f, -0.3f, 0.f);
				m_pTransform->setScale(0.5f, 0.5f, 0.5f);
				break;

			}
		}
	}
}


CPlayerModel* CPlayerModel::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayerModel* pInstance = new CPlayerModel(pDevice);
	if (FAILED(pInstance->Init_PlayerModel()))
		Safe_Release(pInstance);
	return pInstance;
}
