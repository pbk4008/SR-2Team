#include "pch.h"
#include "MeleeMon.h"
#include "Transform.h"
#include "Player.h"
#include "MeleeMon_Idle.h"
#include "MeleeMon_WalkF.h"
#include "MeleeMon_Attack.h"

CMeleeMon::CMeleeMon()
	: m_pBufferCom(nullptr), m_pTexture(nullptr), m_fSpeed(0.f),
	m_bAttack(false), m_iTimer(1), m_eState(STATE::MAX), m_pAnimator(nullptr),
	m_eCurState(STATE::MAX), m_ePreState(STATE::MAX), m_bMoving(false),
	m_pCollision(nullptr), m_pAttackColl(nullptr)
{
	
}

CMeleeMon::CMeleeMon(LPDIRECT3DDEVICE9 pDevice)
	: CMonster(pDevice), m_pBufferCom(nullptr), m_pTexture(nullptr),
	m_fSpeed(0.f), m_bAttack(false), m_iTimer(1), m_eState(STATE::MAX), 
	m_pAnimator(nullptr), m_eCurState(STATE::MAX), m_ePreState(STATE::MAX), 
	m_bMoving(false), m_pCollision(nullptr), m_pAttackColl(nullptr)
{

}

CMeleeMon::CMeleeMon(const CMeleeMon& rhs)
	: CMonster(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTexture(Clone_ComProto<CTexture>(COMPONENTID::MELEEMON_IDLETEX)),
	 m_fSpeed(rhs.m_fSpeed), m_bAttack(rhs.m_bAttack), m_iTimer(1), m_eState(rhs.m_eState), 
	 m_pAnimator(rhs.m_pAnimator), m_eCurState(rhs.m_eCurState), 
	m_ePreState(rhs.m_ePreState), m_bMoving(rhs.m_bMoving), m_pCollision(rhs.m_pCollision), m_pAttackColl(rhs.m_pAttackColl)
{
	SettingAnimator();
	m_eCurState = STATE::IDLE;
	m_pCollision->AddRef();
	m_pCollision->setTransform(m_pTransform);
	m_pAttackColl->setTransform(m_pTransform);
}

CMeleeMon::~CMeleeMon()
{
}

HRESULT CMeleeMon::Init_MeleeMon()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 2.f;

	return S_OK;
}

Engine::_int CMeleeMon::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	Follow(fDeltaTime);
	Attack_Dis(fDeltaTime);

	if (m_bAttack)
		m_eCurState = STATE::ATTACK;
	else
		m_eCurState = STATE::WALKING;

	Change_State();
	iExit = CGameObject::Update_GameObject(fDeltaTime);
	Insert_RenderGroup(RENDERGROUP::ALPHA, this);


	return iExit;
}

void CMeleeMon::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject();

	if (m_pCollision->getHit())
	{
		cout << "¸ó½ºÅÍ Ãæµ¹" << endl;
		m_pCollision->setHit(false);
	}
}

void CMeleeMon::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	m_pCollision->Render_Collision();

	//m_pTexture->Render_Texture();
	m_pAnimator->Render_Animator();
	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

Engine::CGameObject* CMeleeMon::Clone_GameObject()
{
	return new CMeleeMon(*this);
}

HRESULT CMeleeMon::SettingAnimator()
{
	m_pAnimator = Clone_ComProto < CAnimator>(COMPONENTID::ANIMATOR);

	CAnimation* pAnimation = Clone_ComProto<CMeleeMon_Idle>(COMPONENTID::MELEEMON_IDLEANIM);
	m_pAnimator->Insert_Animation(L"MeleeMon_Idle", L"Head", pAnimation);

	CMeleeMon_WalkF* pWalkF = Clone_ComProto<CMeleeMon_WalkF>(COMPONENTID::MELEEMON_WALKANIM);
	m_pAnimator->Insert_Animation(L"MeleeMon_WalkF", L"MeleeMon_Idle", pWalkF, true);

	CMeleeMon_Attack* pAttack = Clone_ComProto<CMeleeMon_Attack>(COMPONENTID::MELEEMON_ATTACKANIM);
	m_pAnimator->Insert_Animation(L"MeleeMon_Attack", L"MeleeMon_Idle", pAttack, true);

	FAILED_CHECK(m_pAnimator->Change_Animation(L"MeleeMon_Idle"));

	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::ANIMATOR, m_pAnimator);

	return S_OK;
}

CMeleeMon* CMeleeMon::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMeleeMon* pInstance = new CMeleeMon(pDevice);

	if (FAILED(pInstance->Init_MeleeMon()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMeleeMon::Change_State()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CMeleeMon::STATE::IDLE:
			m_pAnimator->Change_Animation(L"MeleeMon_Idle");
			break;
		case CMeleeMon::STATE::WALKING:
			m_pAnimator->Change_Animation(L"MeleeMon_WalkF");
			break;
		case CMeleeMon::STATE::ATTACK:
			m_pAnimator->Change_Animation(L"MeleeMon_Attack");
			break;
		}
		m_ePreState = m_eCurState;
	}
}


void CMeleeMon::Follow(const _float& fDeltaTime)
{
	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 playerPos = pObject->getTransform()->getPos();

	Chase_Target(&playerPos, m_fSpeed, fDeltaTime);
}

void CMeleeMon::Attack(const _float& fDeltaTime)
{
	m_iTimer += fDeltaTime;
	if (m_iTimer >= 1.0f)
	{
		cout << "Attack!" << endl;
		m_bAttack = false;
		m_pAttackColl->setActive(false);

		if (m_pAttackColl->getActive())
			m_pAttackColl->Update_Component(fDeltaTime);

		m_pAttackColl->Collison(COLLISIONTAG::PLAYER);

		m_iTimer = 0.f;
	}
}

void CMeleeMon::Attack_Dis(const _float& fDeltaTime)
{
	_vec3	m_vInfo = *m_pTransform->getAxis(VECAXIS::AXIS_POS);
	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 vPos = pObject->getTransform()->getPos();
	_vec3  vDis = m_vInfo - vPos;
	_float fDis = D3DXVec3Length(&vDis);

	if (fDis <= 1.0f)
	{
		m_bAttack = true;
		Attack(fDeltaTime);
		m_pAttackColl->setActive(true);
	}
	else
		m_bAttack = false;
}

HRESULT CMeleeMon::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	// rctex ¹ö¼hÄÞ
	pComponent = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBufferCom->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pComponent);

	//texture idle
	pComponent = m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::MELEEMON_IDLETEX);
	m_pTexture->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::MELEEMON_IDLETEX, pComponent);
	
	//texture walk
	pComponent = m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::MELEEMON_WALKTEX);
	m_pTexture->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::MELEEMON_WALKTEX, pComponent);

	//texture attack
	pComponent = m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::MELEEMON_ATTACKTEX);
	m_pTexture->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::MELEEMON_ATTACKTEX, pComponent);
	
	// collision
	m_pCollision = Clone_ComProto<CCollision>(COMPONENTID::COLLISION);
	m_pCollision->setRadius(1.f);
	m_pCollision->setTag(COLLISIONTAG::MONSTER);
	m_pCollision->setActive(true);
	m_pCollision->setTrigger(COLLISIONTRIGGER::HIT);
	pComponent = m_pCollision;
	Insert_Collision(m_pCollision);
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::COLLISION, pComponent);

	// collision
	m_pAttackColl = Clone_ComProto<CCollision>(COMPONENTID::COLLISION);
	m_pAttackColl->setRadius(1.f);
	m_pAttackColl->setTag(COLLISIONTAG::MONSTER);
	m_pAttackColl->setActive(false);
	pComponent = m_pAttackColl;
	Insert_Collision(m_pAttackColl);

	return S_OK;
}

void CMeleeMon::Free()
{
	Safe_Release(m_pCollision);
	Safe_Release(m_pAttackColl);
	ClearCollisionList();
	Safe_Release(m_pTexture);
	Safe_Release(m_pAnimator);
	Safe_Release(m_pBufferCom);
	CGameObject::Free();
}
