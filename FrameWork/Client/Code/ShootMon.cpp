#include "pch.h"
#include "ShootMon.h"
#include "Transform.h"
#include "Player.h"
#include "ShootMon_Idle.h"
#include "ShootMon_WalkF.h"
#include "ShootMon_Attack.h"

CShootMon::CShootMon()
	: m_pBufferCom(nullptr), m_pTexture(nullptr), m_fSpeed(0.f),
	m_bAttack(false), m_iTimer(1), m_pAnimator(nullptr),
	m_eCurState(STATE::MAX), m_ePreState(STATE::MAX), m_bMoving(false),
	m_pCollision(nullptr), m_pAttackColl(nullptr)
{

}

CShootMon::CShootMon(LPDIRECT3DDEVICE9 pDevice)
	: CMonster(pDevice), m_pBufferCom(nullptr), m_pTexture(nullptr),
	m_fSpeed(0.f), m_bAttack(false), m_iTimer(1),
	m_pAnimator(nullptr), m_eCurState(STATE::MAX), m_ePreState(STATE::MAX),
	m_bMoving(false), m_pCollision(nullptr), m_pAttackColl(nullptr)
{

}

CShootMon::CShootMon(const CShootMon& rhs)
	: CMonster(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTexture(rhs.m_pTexture),
	m_fSpeed(rhs.m_fSpeed), m_bAttack(rhs.m_bAttack), m_iTimer(1),
	m_pAnimator(rhs.m_pAnimator), m_eCurState(rhs.m_eCurState),
	m_ePreState(rhs.m_ePreState), m_bMoving(rhs.m_bMoving), m_pCollision(rhs.m_pCollision), m_pAttackColl(rhs.m_pAttackColl)
{
	SettingAnimator();
	m_eCurState = STATE::IDLE;
	m_pCollision->AddRef();
	m_pCollision->setTransform(m_pTransform);
	m_pAttackColl->setTransform(m_pTransform);
}

CShootMon::~CShootMon()
{

}

HRESULT CShootMon::Init_ShootMon()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 2.f;

	return S_OK;
}

_int CShootMon::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	Follow(fDeltaTime);
	Attack_Dis(fDeltaTime);

	m_fSpeed = 2.f;

	if (GetAsyncKeyState('P'))
		m_fSpeed = 0.f;

	if (m_fSpeed == 0.f)
		m_eCurState = STATE::ATTACK;

	//Change_State();
	iExit = CGameObject::Update_GameObject(fDeltaTime);
	Insert_RenderGroup(RENDERGROUP::ALPHA, this);

	return iExit;
}

void CShootMon::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();

	if (m_pCollision->getHit())
	{
		cout << "¸ó½ºÅÍ Ãæµ¹" << endl;
		m_pCollision->setHit(false);
	}
}

void CShootMon::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	m_pCollision->Render_Collision();

	m_pAnimator->Render_Animator();
	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

CGameObject* CShootMon::Clone_GameObject()
{
	return new CShootMon(*this);
}

HRESULT CShootMon::SettingAnimator()
{
	m_pAnimator = Clone_ComProto < CAnimator>(COMPONENTID::ANIMATOR);

	CAnimation* pAnimation = CShootMon_Idle::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"ShootMon_Idle", L"Head", pAnimation);

	CShootMon_WalkF* pWalkF = CShootMon_WalkF::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"ShootMon_WalkF", L"ShootMon_Idle", pWalkF, true);

	CShootMon_Attack* pAttack = CShootMon_Attack::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"ShootMon_Attack", L"ShootMon_Idle", pAttack, true);

	FAILED_CHECK(m_pAnimator->Change_Animation(L"ShootMon_Idle"));

	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::ANIMATOR, m_pAnimator);

	return S_OK;
}

CShootMon* CShootMon::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CShootMon* pInstance = new CShootMon(pDevice);

	if (FAILED(pInstance->Init_ShootMon()))
		Safe_Release(pInstance);

	return pInstance;
}

void CShootMon::Change_State()
{
	//if (m_ePreState != m_eCurState)
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CShootMon::STATE::IDLE:
			m_pAnimator->Change_Animation(L"ShootMon_Idle");
			break;
		case CShootMon::STATE::WALKING:
			m_pAnimator->Change_Animation(L"ShootMon_WalkF");
			break;
		case CShootMon::STATE::ATTACK:
			m_pAnimator->Change_Animation(L"ShootMon_Attack");
			break;
		}
		m_ePreState = m_eCurState;
	}
}

HRESULT CShootMon::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	// rctex ¹ö¼hÄÞ
	pComponent = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBufferCom->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pComponent);

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

void CShootMon::Follow(const _float& fDeltaTime)
{
	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER);
	_vec3 playerPos = pObject->getTransform()->getPos();

	Chase_Target_Ranged(&playerPos, m_fSpeed, fDeltaTime);
}

void CShootMon::Attack(const _float& fDeltaTime)
{
	m_iTimer += fDeltaTime;
	if (m_iTimer >= 1.0f)
	{
		cout << "Shot!" << endl;
		m_bAttack = false;
		m_iTimer = 0.f;
	}
}

void CShootMon::Attack_Dis(const _float& fDeltaTime)
{
	_vec3	m_vInfo;
	m_pTransform->getAxis(VECAXIS::AXIS_POS, m_vInfo);

	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 vPos = pObject->getTransform()->getPos();
	_vec3  vDis = m_vInfo - vPos;
	_float fDis = D3DXVec3Length(&vDis);

	if (fDis <= 1.0f)
	{
		m_eCurState = STATE::ATTACK;
		Change_State();

		Attack(fDeltaTime);
		m_pAttackColl->setActive(true);
	}
	else if (fDis > 1.0f)
	{
		m_eCurState = STATE::WALKING;
		Change_State();
	}
}

void CShootMon::Free()
{
	Safe_Release(m_pCollision);
	Safe_Release(m_pAttackColl);
	ClearCollisionList();
	Safe_Release(m_pTexture);
	Safe_Release(m_pAnimator);
	Safe_Release(m_pBufferCom);
	CGameObject::Free();
}
