#include "pch.h"
#include "MeleeMon.h"
#include "Transform.h"
#include "Player.h"
#include "Shuriken.h"
#include "MeleeMon_Idle.h"
#include "MeleeMon_WalkF.h"
#include "MeleeMon_Attack.h"
#include "MeleeMon_Death.h"
#include "SphereCollision.h"
#include "Key.h"
#include "Blood.h"

CMeleeMon::CMeleeMon()
	: m_pBufferCom(nullptr), m_pTexture(nullptr), m_fSpeed(0.f),
	m_bAttack(false), m_iTimer(1), m_pAnimator(nullptr),
	m_eCurState(STATE::MAX), m_ePreState(STATE::MAX), m_bMoving(false),
	m_pCollision(nullptr), m_pAttackColl(nullptr), m_iHP(0), m_bTracking(false)
	, m_fAttackDelay(0.f)
{
	
}

CMeleeMon::CMeleeMon(LPDIRECT3DDEVICE9 pDevice)
	: CMonster(pDevice), m_pBufferCom(nullptr), m_pTexture(nullptr),
	m_fSpeed(0.f), m_bAttack(false), m_iTimer(0), 
	m_pAnimator(nullptr), m_eCurState(STATE::MAX), m_ePreState(STATE::MAX), 
	m_bMoving(false), m_pCollision(nullptr), m_pAttackColl(nullptr), m_iHP(0), m_bTracking(false)
	, m_fAttackDelay(0.f)
{

}

CMeleeMon::CMeleeMon(const CMeleeMon& rhs)
	: CMonster(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTexture(rhs.m_pTexture),
	m_fSpeed(rhs.m_fSpeed), m_bAttack(rhs.m_bAttack), m_iTimer(rhs.m_iTimer),
	 m_pAnimator(rhs.m_pAnimator), m_eCurState(rhs.m_eCurState), 
	m_ePreState(rhs.m_ePreState), m_bMoving(rhs.m_bMoving), m_pCollision(nullptr), m_pAttackColl(nullptr),
	m_iHP(rhs.m_iHP), m_bTracking(rhs.m_bTracking), m_fAttackDelay(rhs.m_fAttackDelay)
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
	Insert_ObjCollision(m_pCollision);
	m_pCollision->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::SPHERECOL, pComponent);

	// collision
	m_pAttackColl = Clone_ComProto<CSphereCollision>(COMPONENTID::SPHERECOL);
	m_pAttackColl->setRadius(1.f);
	m_pAttackColl->setTag(COLLISIONTAG::MONSTER);
	m_pAttackColl->setTrigger(COLLISIONTRIGGER::ATTACK);
	m_pAttackColl->setTransform(m_pTransform);
	m_pAttackColl->setActive(false);
	pComponent = m_pAttackColl;
	Insert_ObjCollision(m_pAttackColl);
}

CMeleeMon::~CMeleeMon()
{
}

HRESULT CMeleeMon::Init_MeleeMon()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 2.f;
	m_iHP = 3;

	return S_OK;
}

Engine::_int CMeleeMon::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	m_pTransform->UsingGravity(fDeltaTime);

	_matrix matRot;
	matRot = *ComputeLookAtTarget();
	m_pTransform->setRotate(matRot);

	_vec3	m_vInfo;
	m_vInfo = m_pTransform->getPos();
	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 vPos = pObject->getTransform()->getPos();
	_vec3  vDis = m_vInfo - vPos;
	_float fDis = D3DXVec3Length(&vDis);
	if (fDis <= 8.0f)
		m_bTracking = true;
	if (fDis > 8.0f)
		m_bTracking = false;

	Change_State();
	if (m_eCurState == STATE::DEATH)
	{
		m_bTracking = false;
		Blooding(fDeltaTime);//ÇÇÈê¸®´Â Effect
		if (!lstrcmp(m_pAnimator->getCurrentAnim(), L"MeleeMon_Death"))
		{
			if (!m_pAnimator->getAnimPlay())
			{
				_int iRandNum = rand() % 100;
				if (m_bItemCheck)//Item»ý¼º
				{
					if (iRandNum < 60)//³ª¿Ã È®·ü 60%
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
				m_dwBloodCount = 0;
				return iExit;
			}
		}
	}
	if (m_bTracking&&!static_cast<CPlayer*>(pObject)->getHide())
	{
		Follow(fDeltaTime);
		Attack_Dis(fDeltaTime);
	}
	if (m_pBlood)
		m_pBlood->Update_GameObject(fDeltaTime);
	iExit = CGameObject::Update_GameObject(fDeltaTime);

	if (m_pAttackColl->getActive())
	{
		m_pAttackColl->Update_Component(fDeltaTime);
		m_pAttackColl->Collison(COLLISIONTAG::PLAYER);	
	}

	Insert_RenderGroup(RENDERGROUP::NONALPHA, this);
	return iExit;
}

void CMeleeMon::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();

	_float fDeltaTime = GetOutDeltaTime();

	HitMonster(fDeltaTime);
	if (m_pAttackColl->getActive())
	{
		if (m_pAttackColl->getCollider())
		{
			m_pAttackColl->setActive(false);
			m_bAttack = true;
		}
	}
}

void CMeleeMon::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	m_pCollision->Render_Collision();

	m_pAnimator->Render_Animator();
	m_pBufferCom->Render_Buffer();

	if (m_pBlood)
		m_pBlood->Render_GameObject();

	CGameObject::Render_GameObject();
}

Engine::CGameObject* CMeleeMon::Clone_GameObject()
{
	return new CMeleeMon(*this);
}

void CMeleeMon::ResetObject()
{
	m_bAttack = false;
	m_pAttackColl->setActive(false);

	m_bTracking = false;
	m_iHP = 50;
	m_fAttackDelay = 0.f;

	m_eCurState = STATE::IDLE;
	m_bItemCheck = false;
}

HRESULT CMeleeMon::SettingAnimator()
{
	m_pAnimator = Clone_ComProto < CAnimator>(COMPONENTID::ANIMATOR);

	CAnimation* pAnimation = CMeleeMon_Idle::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"MeleeMon_Idle", L"Head", pAnimation);

	CMeleeMon_WalkF* pWalkF = CMeleeMon_WalkF::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"MeleeMon_WalkF", L"MeleeMon_Idle", pWalkF, true);

	CMeleeMon_Attack* pAttack = CMeleeMon_Attack::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"MeleeMon_Attack", L"MeleeMon_Idle", pAttack, true);

	CMeleeMon_Death* pDeath = CMeleeMon_Death::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"MeleeMon_Death", L"MeleeMon_Idle", pDeath, true);

	m_pAnimator->Connet_Animation(L"MeleeMon_WalkF", L"MeleeMon_Attack");
	m_pAnimator->Connet_Animation(L"MeleeMon_Attack", L"MeleeMon_WalkF");
	m_pAnimator->Connet_Animation(L"MeleeMon_WalkF", L"MeleeMon_Death");
	m_pAnimator->Connet_Animation(L"MeleeMon_Death", L"MeleeMon_WalkF");
	m_pAnimator->Connet_Animation(L"MeleeMon_Attack", L"MeleeMon_Death");
	m_pAnimator->Connet_Animation(L"MeleeMon_Death", L"MeleeMon_Attack");

	FAILED_CHECK(m_pAnimator->Change_Animation(L"MeleeMon_Idle"));

	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::ANIMATOR, m_pAnimator);
	m_pAnimator->AddRef();
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
	//if (m_ePreState != m_eCurState)
	if (m_eCurState != m_ePreState)
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
		case CMeleeMon::STATE::DEATH:
			m_pAnimator->Change_Animation(L"MeleeMon_Death");
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CMeleeMon::HitMonster(const _float& fTimeDelta)
{
	if (m_pCollision->getHit())
	{
		cout << typeid(*(m_pCollision->getCollider()->getTarget())).name() << endl;
		if (typeid(*(m_pCollision->getCollider()->getTarget())) == typeid(CShuriken))
		{
			m_iHP--;
			if (m_iHP <= 0)
				m_eCurState = CMeleeMon::STATE::DEATH;
		}
		else if (typeid(*m_pCollision->getCollider()->getTarget()) == typeid(CPlayer))
			m_eCurState = CMeleeMon::STATE::DEATH;
		m_pCollision->ResetCollision();
	}
	//m_iTimer += fTimeDelta;
	//if (m_iTimer >= 2.0f)//if (m_iTimer >= 1.0f)
	//{
	//	if (m_pCollision->getHit())
	//	{
	//		//m_iHP -= 20;
	//		/*if (m_iHP <= 0)
	//		{
	//			m_eCurState = CMeleeMon::STATE::DEATH;
	//			Change_State();
	//			m_fSpeed = 0.f;
	//		}*/

	//		m_eCurState = CMeleeMon::STATE::DEATH;
	//		Change_State();
	//		m_fSpeed = 0.f;

	//		cout << "Monster got Hit!" << endl;
	//		cout << m_iHP << endl;

	//		m_pCollision->Collison(COLLISIONTAG::PLAYER);
	//		m_pCollision->ResetCollision();
	//		//m_pCollision->setHit(false);

	//		m_iTimer = 0.f;
	//	}
	//}
}
void CMeleeMon::Follow(const _float& fDeltaTime)
{
	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	if (m_pTransform->getBottomY() != pObject->getTransform()->getBottomY())
		return;
	_vec3 playerPos = pObject->getTransform()->getPos();

	
	Chase_Target(&playerPos, m_fSpeed, fDeltaTime);
}

void CMeleeMon::Attack(const _float& fDeltaTime)
{
	m_iTimer += fDeltaTime;
	if (m_iTimer >= 2.0f)
	{
		cout << "Attack!" << endl;
		m_pAttackColl->setActive(false);
		m_iTimer = 0.f;
	}
}

void CMeleeMon::Attack_Dis(const _float& fDeltaTime)
{
	_vec3	m_vInfo;
	m_pTransform->getAxis(VECAXIS::AXIS_POS, m_vInfo);

	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 vPos = pObject->getTransform()->getPos();
	_vec3  vDis = m_vInfo - vPos;
	_float fDis = D3DXVec3Length(&vDis);

	if (fDis <= 1.0f)
	{
		if (!m_bAttack)
		{
			m_eCurState = STATE::ATTACK;
			m_pAttackColl->setActive(true);
			//Attack(fDeltaTime);
		}
		else
		{
			m_fAttackDelay += fDeltaTime;
			if (m_fAttackDelay > 2.f)
			{
				m_fAttackDelay = 0.f;
				m_bAttack = false;
			}
		}
		//m_pAttackColl->setActive(true);
	}
	else if (fDis > 1.0f)
	{
		m_eCurState = STATE::WALKING;
		m_pAttackColl->setActive(false);
	}
}

HRESULT CMeleeMon::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	// rctex ¹ö¼hÄÞ
	pComponent = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBufferCom->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pComponent);

	return S_OK;
}

void CMeleeMon::Free()
{
	CMonster::Free();
	Safe_Release(m_pCollision);
	Safe_Release(m_pAttackColl);
	Safe_Release(m_pTexture);
	Safe_Release(m_pAnimator);
	Safe_Release(m_pBufferCom);
};