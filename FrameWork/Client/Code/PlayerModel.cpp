#include "pch.h"
#include "PlayerModel.h"
#include "Animator.h"
#include "Player_AttackAnim.h"
#include "Player_IdleAnim.h"
#include "Player_Walk.h"
CPlayerModel::CPlayerModel() : m_pBufferCom(nullptr), m_eState(CPlayer::STATE::MAX), m_pAnimator(nullptr)
{
}

CPlayerModel::CPlayerModel(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pBufferCom(nullptr), m_pAnimator(nullptr), m_eState(CPlayer::STATE::MAX)
{
}

CPlayerModel::CPlayerModel(const CPlayerModel& rhs) : CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pAnimator(rhs.m_pAnimator), m_eState(rhs.m_eState)
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
	return S_OK;
}

_int CPlayerModel::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	m_pTransform->setScale(1.f, 0.8f, 0.5f);
	Insert_RenderGroup(RENDERGROUP::ALPHA, this);

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

CGameObject* CPlayerModel::Clone_GameObject()
{
	return new CPlayerModel(*this);
}

HRESULT CPlayerModel::SettingAnimator()
{
	m_pAnimator = Clone_ComProto<CAnimator>(COMPONENTID::ANIMATOR);

	CAnimation* pAnim = Clone_ComProto<CPlayerIdleAnim>(COMPONENTID::PLAYER_IDLEANIM);
	m_pAnimator->Insert_Animation(L"Player_Idle", L"Head", pAnim);

	CPlayer_AttackAnim* pAtk = Clone_ComProto<CPlayer_AttackAnim>(COMPONENTID::PLAYER_ATTACKANIM);
	pAtk->setTransform(m_pTransform);
	pAnim = pAtk;
	m_pAnimator->Insert_Animation(L"Player_Attack", L"Player_Idle", pAnim, true);

	CPlayerWalk* pWalk = Clone_ComProto<CPlayerWalk>(COMPONENTID::PLAYER_WALKANIM);
	pWalk->setTransform(m_pTransform);
	pAnim = pWalk;
	m_pAnimator->Insert_Animation(L"Player_Walk", L"Player_Idle", pAnim, true);
	
	FAILED_CHECK(m_pAnimator->Change_Animation(L"Player_Idle"));

	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::ANIMATOR, m_pAnimator);

	return S_OK;
}

CPlayer::STATE CPlayerModel::Act()
{
	switch (m_eState)
	{
	case CPlayer::STATE::IDLE:
		m_pAnimator->Change_Animation(L"Player_Idle");
		break;
	case CPlayer::STATE::ATTACK:
		if (!lstrcmp(m_pAnimator->getCurrentAnim(), L"Player_Attack"))
		{
			if (!m_pAnimator->getAnimPlay())
				m_eState = CPlayer::STATE::IDLE;
		}
		else
			m_pAnimator->Change_Animation(L"Player_Attack");
		break;
	case CPlayer::STATE::WALK:
		m_pAnimator->Change_Animation(L"Player_Walk");
		break;
	}
	return m_eState;
}


CPlayerModel* CPlayerModel::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayerModel* pInstance = new CPlayerModel(pDevice);
	if (FAILED(pInstance->Init_PlayerModel()))
		Safe_Release(pInstance);
	return pInstance;
}
