#include "pch.h"
#include "PlayerModel.h"
#include "Player_AttackAnim.h"
CPlayerModel::CPlayerModel() : m_pBufferCom(nullptr), m_pAtkAnim(nullptr), m_bAttack(false)
{
}

CPlayerModel::CPlayerModel(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pBufferCom(nullptr), m_pAtkAnim(nullptr), m_bAttack(false)
{
}

CPlayerModel::CPlayerModel(const CPlayerModel& rhs) : CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pAtkAnim(rhs.m_pAtkAnim), m_bAttack(rhs.m_bAttack)
{
	m_pBufferCom->AddRef();
	m_pAtkAnim->AddRef();
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

	m_pTransform->setScale(0.8f, 0.8f, 0.5f);
	//m_pTransform->setPos(-0.1f, 0.f, 0.f);
	if (m_bAttack)
	{
		m_pAtkAnim->Update_Component(fDeltaTime);
		m_pAtkAnim->setPlay(true);
	}

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
	m_pAtkAnim->Render_Animation();
	if (!m_pAtkAnim->getPlay())
	{
		m_bAttack = false;
		m_pAtkAnim->ResetTimer();
	}
	m_pBufferCom->Render_Buffer();
}

HRESULT CPlayerModel::Add_Component()
{
	CGameObject::Add_Component();

	CComponent* pCom = nullptr;

	pCom = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBufferCom->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pCom);

	pCom = m_pAtkAnim = Clone_ComProto<CPlayer_AttackAnim>(COMPONENTID::PLAYER_ATTACKANIM);
	m_pAtkAnim->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::PLAYER_ATTACKANIM, pCom);

	return S_OK;
}

void CPlayerModel::Free()
{
	Safe_Release(m_pAtkAnim);
	CGameObject::Free();
	Safe_Release(m_pBufferCom);
}

void CPlayerModel::setTarget(CTransform* pTarget)
{
	m_pTransform->setParent(pTarget);
	m_pTransform->setPos(-0.15f, 0.f, 0.f);
	m_pAtkAnim->setTransform(m_pTransform);
}

CGameObject* CPlayerModel::Clone_GameObject()
{
	return new CPlayerModel(*this);
}

CPlayerModel* CPlayerModel::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayerModel* pInstance = new CPlayerModel(pDevice);
	if (FAILED(pInstance->Init_PlayerModel()))
		Safe_Release(pInstance);
	return pInstance;
}
