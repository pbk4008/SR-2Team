#include "pch.h"
#include "FlyMon.h"
#include "Transform.h"
#include "Player.h"
#include "FlyMon_Idle.h"
#include "FlyMon_WalkF.h"
#include "FlyMon_Attack.h"
#include "FlyMon_Death.h"
#include "SphereCollision.h"
#include "Key.h"
CFlyMon::CFlyMon()
	: m_pBufferCom(nullptr), m_pTexture(nullptr), m_fSpeed(0.f),
	m_bAttack(false), m_iTimer(0), m_pAnimator(nullptr),
	m_eCurState(STATE::MAX), m_ePreState(STATE::MAX), m_bMoving(false),
	m_pCollision(nullptr), m_pAttackColl(nullptr), m_iHP(0), m_bTracking(false)
{

}

CFlyMon::CFlyMon(LPDIRECT3DDEVICE9 pDevice)
	: CMonster(pDevice), m_pBufferCom(nullptr), m_pTexture(nullptr),
	m_fSpeed(0.f), m_bAttack(false), m_iTimer(0),
	m_pAnimator(nullptr), m_eCurState(STATE::MAX), m_ePreState(STATE::MAX),
	m_bMoving(false), m_pCollision(nullptr), m_pAttackColl(nullptr), m_iHP(0), m_bTracking(false)
{

}

CFlyMon::CFlyMon(const CFlyMon& rhs)
	: CMonster(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTexture(rhs.m_pTexture),
	m_fSpeed(rhs.m_fSpeed), m_bAttack(rhs.m_bAttack), m_iTimer(rhs.m_iTimer),
	m_pAnimator(rhs.m_pAnimator), m_eCurState(rhs.m_eCurState),
	m_ePreState(rhs.m_ePreState), m_bMoving(rhs.m_bMoving), m_pCollision(nullptr), m_pAttackColl(nullptr),
	m_iHP(rhs.m_iHP), m_bTracking(rhs.m_bTracking)
{
	m_pBufferCom->AddRef();

	SettingAnimator();
	m_eCurState = STATE::IDLE;

	CComponent* pComponent = nullptr;

	// collision
	m_pCollision = Clone_ComProto<CSphereCollision>(COMPONENTID::SPHERECOL);
	m_pCollision->setRadius(1.f);
	m_pCollision->setTag(COLLISIONTAG::MONSTER);
	m_pCollision->setActive(true);
	m_pCollision->setTrigger(COLLISIONTRIGGER::HIT);
	m_pCollision->setTransform(m_pTransform);
	pComponent = m_pCollision;
	Insert_Collision(m_pCollision);
	m_pCollision->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::SPHERECOL, pComponent);

	// collision
	m_pAttackColl = Clone_ComProto<CSphereCollision>(COMPONENTID::SPHERECOL);
	m_pAttackColl->setRadius(2.f);
	m_pAttackColl->setTag(COLLISIONTAG::MONSTER);
	m_pAttackColl->setTrigger(COLLISIONTRIGGER::ATTACK);
	m_pAttackColl->setActive(false);
	m_pAttackColl->setTransform(m_pTransform);
	pComponent = m_pAttackColl;
	Insert_Collision(m_pAttackColl);
}

CFlyMon::~CFlyMon()
{
}

HRESULT CFlyMon::Init_FlyMon()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 2.f;

	return S_OK;
}

Engine::_int CFlyMon::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	m_pTransform->UsingGravity(fDeltaTime);

	_matrix matRot;
	matRot = *ComputeLookAtTarget();
	m_pTransform->setRotate(matRot);

	_vec3	m_vInfo;
	m_pTransform->getAxis(VECAXIS::AXIS_POS, m_vInfo);
	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 vPos = pObject->getTransform()->getPos();
	_vec3  vDis = m_vInfo - vPos;
	_float fDis = D3DXVec3Length(&vDis);
	if (fDis <= 12.0f)
		m_bTracking = true;
	if (fDis >= 12.0f)
		m_bTracking = false;

	if (m_bTracking)
	{
		if (m_eCurState == STATE::DEATH)
		{
			if (!lstrcmp(m_pAnimator->getCurrentAnim(), L"FlyMon_Death"))
			{
				if (!m_pAnimator->getAnimPlay())
				{
					_int iRandNum = rand() % 100;
					if (m_bItemCheck)//Item»ý¼º
					{
						if (iRandNum < 60)
						{
							CGameObject* pKey = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::KEY);
							if (!pKey)
							{
								pKey = Clone_ObjProto<CKey>(GAMEOBJECTID::KEY);
								Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::KEY, pKey);
							}
							pKey->getTransform()->setPos(m_pTransform->getPos());
						}
					}
					setActive(false);
					return iExit;
				}
			}
		}

		Follow(fDeltaTime);
		Attack_Dis(fDeltaTime);
	}


	iExit = CGameObject::Update_GameObject(fDeltaTime);
	Insert_RenderGroup(RENDERGROUP::NONALPHA, this);

	return iExit;
}

void CFlyMon::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();

	_float fDeltaTime = GetOutDeltaTime();

	HitMonster(fDeltaTime);
}

void CFlyMon::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	m_pCollision->Render_Collision();

	m_pAnimator->Render_Animator();
	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

Engine::CGameObject* CFlyMon::Clone_GameObject()
{
	return new CFlyMon(*this);
}

HRESULT CFlyMon::SettingAnimator()
{
	m_pAnimator = Clone_ComProto < CAnimator>(COMPONENTID::ANIMATOR);

	CAnimation* pAnimation = CFlyMon_Idle::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"FlyMon_Idle", L"Head", pAnimation);

	CFlyMon_WalkF* pWalkF = CFlyMon_WalkF::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"FlyMon_WalkF", L"FlyMon_Idle", pWalkF, true);

	CFlyMon_Attack* pAttack = CFlyMon_Attack::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"FlyMon_Attack", L"FlyMon_Idle", pAttack, true);

	CFlyMon_Death* pDeath = CFlyMon_Death::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"FlyMon_Death", L"FlyMon_Idle", pDeath, true);

	m_pAnimator->Connet_Animation(L"FlyMon_WalkF", L"FlyMon_Attack");
	m_pAnimator->Connet_Animation(L"FlyMon_Attack", L"FlyMon_WalkF");
	m_pAnimator->Connet_Animation(L"FlyMon_WalkF", L"FlyMon_Death");
	m_pAnimator->Connet_Animation(L"FlyMon_Death", L"FlyMon_WalkF");
	m_pAnimator->Connet_Animation(L"FlyMon_Attack", L"FlyMon_Death");
	m_pAnimator->Connet_Animation(L"FlyMon_Death", L"FlyMon_Attack");

	FAILED_CHECK(m_pAnimator->Change_Animation(L"FlyMon_Idle"));

	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::ANIMATOR, m_pAnimator);

	return S_OK;
}


CFlyMon* CFlyMon::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFlyMon* pInstance = new CFlyMon(pDevice);

	if (FAILED(pInstance->Init_FlyMon()))
		Safe_Release(pInstance);

	return pInstance;
}

void CFlyMon::Change_State()
{
	//if (m_ePreState != m_eCurState)
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CFlyMon::STATE::IDLE:
			m_pAnimator->Change_Animation(L"FlyMon_Idle");
			break;
		case CFlyMon::STATE::WALKING:
			m_pAnimator->Change_Animation(L"FlyMon_WalkF");
			break;
		case CFlyMon::STATE::ATTACK:
			m_pAnimator->Change_Animation(L"FlyMon_Attack");
			break;
		case CFlyMon::STATE::DEATH:
			m_pAnimator->Change_Animation(L"FlyMon_Death");
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CFlyMon::HitMonster(const _float& fTimeDelta)
{
	m_iTimer += fTimeDelta;
	if (m_iTimer >= 2.0f)
		//if (m_iTimer >= 1.0f)
	{
		if (m_pCollision->getHit())
		{
			m_eCurState = CFlyMon::STATE::DEATH;
			Change_State();
			m_fSpeed = 0.f;

			cout << "Monster got Hit!" << endl;
			cout << m_iHP << endl;

			m_pCollision->Collison(COLLISIONTAG::PLAYER);
			m_pCollision->ResetCollision();
			m_pCollision->setHit(false);

			m_iTimer = 0.f;
		}
	}
}

void CFlyMon::Follow(const _float& fDeltaTime)
{
	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 playerPos = pObject->getTransform()->getPos();

	Chase_Target_Fly(&playerPos, m_fSpeed, fDeltaTime);
}

void CFlyMon::Attack(const _float& fDeltaTime)
{
	m_iTimer += fDeltaTime;
	if (m_iTimer >= 1.0f)
	{
		cout << "Attack!" << endl;

		if (m_pAttackColl->getActive())
			m_pAttackColl->Update_Component(fDeltaTime);

		m_pAttackColl->Collison(COLLISIONTAG::PLAYER);

		m_pAttackColl->setActive(false);
		m_iTimer = 0.f;
	}
}

void CFlyMon::Attack_Dis(const _float& fDeltaTime)
{
	_vec3	m_vInfo;
	m_pTransform->getAxis(VECAXIS::AXIS_POS, m_vInfo);

	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 vPos = pObject->getTransform()->getPos();
	_vec3  vDis = m_vInfo - vPos;
	_float fDis = D3DXVec3Length(&vDis);

	if (fDis <= 2.5f)
	{
		m_eCurState = STATE::ATTACK;
		Change_State();

		Attack(fDeltaTime);
		m_pAttackColl->setActive(true);
	}
	else if (fDis > 2.5f)
	{
		m_eCurState = STATE::WALKING;
		Change_State();
	}
}

HRESULT CFlyMon::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	// rctex ¹ö¼hÄÞ
	pComponent = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBufferCom->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pComponent);

	return S_OK;
}

void CFlyMon::Free()
{
	CMonster::Free();
	Safe_Release(m_pCollision);
	Safe_Release(m_pAttackColl);
	Safe_Release(m_pTexture);
	Safe_Release(m_pAnimator);
	Safe_Release(m_pBufferCom);
}