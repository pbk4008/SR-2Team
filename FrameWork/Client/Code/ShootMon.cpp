#include "pch.h"
#include "ShootMon.h"
#include "Transform.h"
#include "Player.h"
#include "ShootMon_Idle.h"
#include "ShootMon_WalkF.h"
#include "ShootMon_Attack.h"
#include "ShootMon_Death.h"
#include "MonBullet.h"
#include "SphereCollision.h"
#include "Key.h"
#include "Shuriken.h"

CShootMon::CShootMon()
	: m_pBufferCom(nullptr), m_pTexture(nullptr), m_fSpeed(0.f),
	m_bAttack(false), m_iTimer(0), m_pAnimator(nullptr),
	m_eCurState(STATE::MAX), m_ePreState(STATE::MAX), m_bMoving(false),
	m_pCollision(nullptr),m_iHP(0), m_pMonBullet(nullptr), m_bTracking(false), m_fAttackDelay(0.f)
{
}

CShootMon::CShootMon(LPDIRECT3DDEVICE9 pDevice)
	: CMonster(pDevice), m_pBufferCom(nullptr), m_pTexture(nullptr),
	m_fSpeed(0.f), m_bAttack(false), m_iTimer(0),
	m_pAnimator(nullptr), m_eCurState(STATE::MAX), m_ePreState(STATE::MAX),
	m_bMoving(false), m_pCollision(nullptr), m_iHP(0), m_pMonBullet(nullptr), m_bTracking(false)
	, m_fAttackDelay(0.f)
{

}

CShootMon::CShootMon(const CShootMon& rhs)
	: CMonster(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTexture(rhs.m_pTexture),
	m_fSpeed(rhs.m_fSpeed), m_bAttack(rhs.m_bAttack), m_iTimer(rhs.m_iTimer),
	m_pAnimator(rhs.m_pAnimator), m_eCurState(rhs.m_eCurState),
	m_ePreState(rhs.m_ePreState), m_bMoving(rhs.m_bMoving), m_pCollision(nullptr),
	m_iHP(rhs.m_iHP), m_pMonBullet(rhs.m_pMonBullet), m_bTracking(rhs.m_bTracking), m_fAttackDelay(rhs.m_fAttackDelay)
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
	/*m_pAttackColl = Clone_ComProto<CSphereCollision>(COMPONENTID::SPHERECOL);
	m_pAttackColl->setRadius(1.f);
	m_pAttackColl->setTag(COLLISIONTAG::MONSTER);
	m_pAttackColl->setTrigger(COLLISIONTRIGGER::ATTACK);
	m_pAttackColl->setActive(false);
	pComponent = m_pAttackColl;
	Insert_Collision(m_pAttackColl);*/
}

CShootMon::~CShootMon()
{

}

HRESULT CShootMon::Init_ShootMon()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 5.f;
	m_iHP = 3;
	return S_OK;
}

_int CShootMon::Update_GameObject(const _float& fDeltaTime)
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
	if (fDis <= 13.0f)
		m_bTracking = true;
	if (fDis > 13.0f)
		m_bTracking = false;

	Change_State();
	if (m_eCurState == STATE::DEATH)
	{
		m_bTracking = false;
		Blooding(fDeltaTime);//ÇÇÈê¸®´Â Effect
		if (!lstrcmp(m_pAnimator->getCurrentAnim(), L"ShootMon_Death"))
		{
			if (!m_pAnimator->getAnimPlay())
			{
				_int iRandNum = rand() % 100;
				if (true)//Item»ý¼º
				{
					if (iRandNum < 100)//³ª¿Ã È®·ü 60%
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
	if (m_bTracking)
	{
		Follow(fDeltaTime);
		Attack_Dis(fDeltaTime);
	}

	iExit = CGameObject::Update_GameObject(fDeltaTime);

	/*if (m_pAttackColl->getActive())
		m_pAttackColl->Collison(COLLISIONTAG::PLAYER);*/

	Insert_RenderGroup(RENDERGROUP::ALPHA, this);
	return iExit;
}

void CShootMon::LateUpdate_GameObject()
{

	CGameObject::LateUpdate_GameObject();

	_float fDeltaTime = GetOutDeltaTime();

	HitMonster(fDeltaTime);
	/*if (m_pAttackColl->getActive())
	{
		if (m_pAttackColl->getCollider())
		{
			m_pAttackColl->setActive(false);
			m_bAttack = true;
		}
	}*/
	//m_bAttack = true;
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

void CShootMon::ResetObject()
{
	m_bAttack = false;
	//m_pAttackColl->setActive(false);

	m_bTracking = false;
	m_iHP = 50;
	m_fAttackDelay = 0.f;

	m_eCurState = STATE::IDLE;
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

	CShootMon_Death* pDeath = CShootMon_Death::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"ShootMon_Death", L"ShootMon_Idle", pDeath, true);

	m_pAnimator->Connet_Animation(L"ShootMon_WalkF", L"ShootMon_Attack");
	m_pAnimator->Connet_Animation(L"ShootMon_Attack", L"ShootMon_WalkF");
	m_pAnimator->Connet_Animation(L"ShootMon_WalkF", L"ShootMon_Death");
	m_pAnimator->Connet_Animation(L"ShootMon_Death", L"ShootMon_WalkF");
	m_pAnimator->Connet_Animation(L"ShootMon_Attack", L"ShootMon_Death");
	m_pAnimator->Connet_Animation(L"ShootMon_Death", L"ShootMon_Attack");

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

CBullet* CShootMon::Shoot(GAMEOBJECTID eID)
{
	_vec3 vLook, vPos;
	_float fAngle;
	vPos = m_pTransform->getPos();

	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 playerPos = pObject->getTransform()->getPos();

	m_pTransform->getAxis(VECAXIS::AXIS_LOOK, vLook);
	CGameObject* pBullet = GetGameObject(LAYERID::GAME_LOGIC, eID);

	if (!pBullet)
			pBullet = Clone_ObjProto<CMonBullet>(eID);

	_vec3 vDir = playerPos - vPos;
	D3DXVec3Normalize(&vDir, &vDir);

	static_cast<CMonBullet*>(pBullet)->setPos(vPos);
	static_cast<CMonBullet*>(pBullet)->setLook(vDir);
	static_cast<CMonBullet*>(pBullet)->setTarget(playerPos);

	return static_cast<CBullet*>(pBullet);
}

void CShootMon::HitMonster(const _float& fTimeDelta)
{
	if (m_pCollision->getHit())
	{
		cout << typeid(*(m_pCollision->getCollider()->getTarget())).name() << endl;
		if (typeid(*(m_pCollision->getCollider()->getTarget())) == typeid(CShuriken))
		{
			m_iHP--;
			if (m_iHP <= 0)
				m_eCurState = CShootMon::STATE::DEATH;
		}
		else if (typeid(*m_pCollision->getCollider()) == typeid(CPlayer))
			m_eCurState = CShootMon::STATE::DEATH;
		m_pCollision->ResetCollision();
	}
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
		case CShootMon::STATE::DEATH:
			m_pAnimator->Change_Animation(L"ShootMon_Death");
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

	return S_OK;
}

void CShootMon::Follow(const _float& fDeltaTime)
{
	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 playerPos = pObject->getTransform()->getPos();

	Chase_Target_Ranged(&playerPos, m_fSpeed, fDeltaTime);
}

void CShootMon::Attack_Dis(const _float& fDeltaTime)
{
	_vec3	m_vInfo;
	m_pTransform->getAxis(VECAXIS::AXIS_POS, m_vInfo);
	CBullet* pBullet = nullptr;
	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 vPos = pObject->getTransform()->getPos();
	_vec3  vDis = m_vInfo - vPos;
	_float fDis = D3DXVec3Length(&vDis);

	if (fDis <= 10.0f)
	{
			m_eCurState = STATE::ATTACK;
			m_fAttackDelay += fDeltaTime;
			if (m_fAttackDelay > 2.f)
			{
				pBullet = Shoot(GAMEOBJECTID::MONBULLET);
				Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONBULLET, pBullet);
				m_fAttackDelay = 0.f;
			}
	}
	else if (fDis > 10.0f)
	{
		m_eCurState = STATE::WALKING;
		//m_pAttackColl->setActive(false);
	}
}

void CShootMon::Free()
{
	CMonster::Free();
	Safe_Release(m_pCollision);
	//Safe_Release(m_pAttackColl);
	Safe_Release(m_pTexture);
	Safe_Release(m_pAnimator);
	Safe_Release(m_pBufferCom);
}
