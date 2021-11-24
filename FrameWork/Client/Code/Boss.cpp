#include "pch.h"
#include "Boss.h"
#include "Transform.h"
#include "Player.h"
#include "Boss_IdleAnim.h"
#include "Boss_WalkAnim.h"
#include "Boss_MeleeAnim.h"
#include "Boss_RangeAnim.h"
#include "Boss_DeathAnim.h"
#include "Boss_ChargeAnim.h"
#include "Fireball.h"

CBoss::CBoss()
	: m_pBufferCom(nullptr), m_pTexture(nullptr), m_pAnimator(nullptr),
	m_eCurState(STATE::MAX), m_ePreState(STATE::MAX), m_pCollision(nullptr), m_pAttackColl(nullptr),
	m_iHP(0), m_bAttack(false), m_bMoving(false), m_iTimer(0), m_fSpeed(0.f), m_iAttackNumber(0),
	m_bChargeAttack(false), m_fChargeTime (0.f), m_fFireballTimer(0.f), m_bHPHit(false)
{

}

CBoss::CBoss(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pBufferCom(nullptr), m_pTexture(nullptr), m_pAnimator(nullptr),
	m_eCurState(STATE::MAX), m_ePreState(STATE::MAX), m_pCollision(nullptr), m_pAttackColl(nullptr),
	m_iHP(0), m_bAttack(false), m_bMoving(false), m_iTimer(0), m_fSpeed(0.f), m_iAttackNumber(0),
	m_bChargeAttack(false), m_fChargeTime(0.f), m_fFireballTimer(0.f), m_bHPHit(false)
{

}

CBoss::CBoss(const CBoss& rhs)
	: CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTexture(rhs.m_pTexture),
	m_fSpeed(rhs.m_fSpeed), m_bAttack(rhs.m_bAttack), m_iTimer(rhs.m_iTimer),
	m_pAnimator(rhs.m_pAnimator), m_eCurState(STATE::MAX), m_ePreState(STATE::MAX),
	m_bMoving(rhs.m_bMoving), m_pCollision(nullptr), m_pAttackColl(nullptr), m_iHP(rhs.m_iHP),
	m_iAttackNumber(rhs.m_iAttackNumber), m_bChargeAttack(rhs.m_bChargeAttack), m_fChargeTime(rhs.m_fChargeTime),
	m_fFireballTimer(rhs.m_fFireballTimer), m_bHPHit(rhs.m_bHPHit)
{
	SettingAnimator();
	m_eCurState = STATE::IDLE;

	CComponent* pComponent = nullptr;

	m_iHP = 1000;


	// collision
	m_pCollision = Clone_ComProto<CSphereCollision>(COMPONENTID::SPHERECOL);
	m_pCollision->setRadius(1.f);
	m_pCollision->setTag(COLLISIONTAG::MONSTER);
	m_pCollision->setActive(true);
	m_pCollision->setTrigger(COLLISIONTRIGGER::HIT);
	m_pCollision->setTransform(m_pTransform);
	pComponent = m_pCollision;
	Insert_ObjCollision(m_pCollision);
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::SPHERECOL, pComponent);

	// collision
	m_pAttackColl = Clone_ComProto<CSphereCollision>(COMPONENTID::SPHERECOL);
	m_pAttackColl->setRadius(1.f);
	m_pAttackColl->setTag(COLLISIONTAG::MONSTER);
	m_pAttackColl->setActive(false);
	pComponent = m_pAttackColl;
	Insert_ObjCollision(m_pAttackColl);
}

CBoss::~CBoss()
{

}

HRESULT CBoss::Init_Boss()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 5.f;

	m_iHP = 1000;
	
	return S_OK;
}

_int CBoss::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	m_pTransform->UsingGravity(fDeltaTime);

	/*_vec3 vScale(2.f,2.f,2.f);
	m_pTransform->setScale(vScale);*/

	HPCheck();		///hp checks into pattern --> m_eCurState

	if (m_eCurState == STATE::DEATH)
	{
		if (!lstrcmp(m_pAnimator->getCurrentAnim(), L"Boss_Death"))
		{
			if (!m_pAnimator->getAnimPlay())
			{
				setActive(false);
				return iExit;
			}
		}
	}

	Follow(fDeltaTime); /// into chase and chase range --> follow distance
	Attack_Dis(fDeltaTime); /// stance determines attack distance

	Change_State();

	iExit = CGameObject::Update_GameObject(fDeltaTime);
	Insert_RenderGroup(RENDERGROUP::ALPHA, this);

	return iExit;
}

void CBoss::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();

	_float fDeltaTime = GetOutDeltaTime();

	HitBoss(fDeltaTime);
	HitPlayer(fDeltaTime);
}

void CBoss::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pAnimator->Render_Animator();
	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
	m_pCollision->Render_Collision();

	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

CGameObject* CBoss::Clone_GameObject()
{
	return new CBoss(*this);
}

HRESULT CBoss::SettingAnimator()
{
	m_pAnimator = Clone_ComProto < CAnimator>(COMPONENTID::ANIMATOR);

	CAnimation* pAnimation = CBoss_IdleAnim::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"Boss_Idle", L"Head", pAnimation);

	CBoss_WalkAnim* pWalk = CBoss_WalkAnim::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"Boss_Move", L"Boss_Idle", pWalk, true);

	CBoss_MeleeAnim* pMelee = CBoss_MeleeAnim::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"Boss_Melee", L"Boss_Idle", pMelee, true);

	CBoss_RangeAnim* pRange = CBoss_RangeAnim::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"Boss_Range", L"Boss_Idle", pRange, true);

	CBoss_DeathAnim* pDeath = CBoss_DeathAnim::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"Boss_Death", L"Boss_Idle", pDeath, true);

	CBoss_ChargeAnim* pCharge = CBoss_ChargeAnim::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"Boss_Charge", L"Boss_Idle", pCharge, true);

	m_pAnimator->Connet_Animation(L"Boss_Move", L"Boss_Melee");
	m_pAnimator->Connet_Animation(L"Boss_Melee", L"Boss_Move");

	m_pAnimator->Connet_Animation(L"Boss_Move", L"Boss_Range");
	m_pAnimator->Connet_Animation(L"Boss_Range", L"Boss_Move");

	m_pAnimator->Connet_Animation(L"Boss_Melee", L"Boss_Range");
	m_pAnimator->Connet_Animation(L"Boss_Range", L"Boss_Melee");

	m_pAnimator->Connet_Animation(L"Boss_Move", L"Boss_Death");
	m_pAnimator->Connet_Animation(L"Boss_Death", L"Boss_Move");

	m_pAnimator->Connet_Animation(L"Boss_Melee", L"Boss_Death");
	m_pAnimator->Connet_Animation(L"Boss_Death", L"Boss_Melee");

	m_pAnimator->Connet_Animation(L"Boss_Range", L"Boss_Death");
	m_pAnimator->Connet_Animation(L"Boss_Death", L"Boss_Range");

	FAILED_CHECK(m_pAnimator->Change_Animation(L"Boss_Idle"));

	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::ANIMATOR, m_pAnimator);

	return S_OK;
}

void CBoss::Change_State()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CBoss::STATE::IDLE:
			m_pAnimator->Change_Animation(L"Boss_Idle");
			break;
		case CBoss::STATE::WALKING:
			m_pAnimator->Change_Animation(L"Boss_Move");
			break;
		case CBoss::STATE::MELEE:
			m_pAnimator->Change_Animation(L"Boss_Melee");
			break;
		case CBoss::STATE::RANGE:
			m_pAnimator->Change_Animation(L"Boss_Range");
			break;
		case CBoss::STATE::CHARGE:
			m_pAnimator->Change_Animation(L"Boss_Charge");
			break;
		case CBoss::STATE::DEATH:
			m_pAnimator->Change_Animation(L"Boss_Death");
			break;
		}
		m_ePreState = m_eCurState;
	}
}

CBoss* CBoss::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBoss* pInstance = new CBoss(pDevice);

	if (FAILED(pInstance->Init_Boss()))
		Safe_Release(pInstance);

	return pInstance;
}

HRESULT CBoss::Add_Component()
{
	CGameObject::Add_Component();
	CComponent* pComponent = nullptr;

	// rctex ¹ö¼hÄÞ
	pComponent = m_pBufferCom = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	m_pBufferCom->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pComponent);

	//// collision
	//m_pCollision = Clone_ComProto<CSphereCollision>(COMPONENTID::SPHERECOL);
	//m_pCollision->setRadius(1.f);
	//m_pCollision->setTag(COLLISIONTAG::MONSTER);
	//m_pCollision->setActive(true);
	//m_pCollision->setTrigger(COLLISIONTRIGGER::HIT);
	//m_pCollision->setTransform(m_pTransform);
	//pComponent = m_pCollision;
	//Insert_Collision(m_pCollision);
	//m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::SPHERECOL, pComponent);

	//// collision
	//m_pAttackColl = Clone_ComProto<CSphereCollision>(COMPONENTID::SPHERECOL);
	//m_pAttackColl->setRadius(1.f);
	//m_pAttackColl->setTag(COLLISIONTAG::MONSTER);
	//m_pAttackColl->setActive(false);
	//pComponent = m_pAttackColl;
	//Insert_Collision(m_pAttackColl);

	return S_OK;
}

void CBoss::Follow(const _float& fDeltaTime)
{
	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 playerPos = pObject->getTransform()->getPos();

	if (m_eCurState == STATE::MELEE)
		Chase(&playerPos, m_fSpeed, fDeltaTime);

	if (m_eCurState == STATE::RANGE)
		ChaseRange(&playerPos, m_fSpeed, fDeltaTime);

	if (m_eCurState == STATE::CHARGE)
		ChaseCharge(&playerPos, m_fSpeed, fDeltaTime);
}

void CBoss::MeleeAttack(const _float& fDeltaTime)
{
	m_eCurState = STATE::MELEE;
	Change_State();
}

void CBoss::RangeAttack(const _float& fDeltaTime)
{
	m_eCurState = STATE::RANGE;
	Change_State();

	m_fFireballTimer += fDeltaTime;
	if (m_fFireballTimer >= 1.0f)
	{
		CBullet* pBullet = nullptr;
		pBullet = Fireball(GAMEOBJECTID::FIREBALL);
		Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONBULLET, pBullet);

		m_fFireballTimer = 0.f;
	}
}

void CBoss::ChargeTarget(const _float& fDeltaTime)
{
	if (m_bChargeAttack)
	{
		_vec3 vPos;
		vPos = m_pTransform->getPos();

		CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
		_vec3 playerPos = pObject->getTransform()->getPos();

		_vec3 vDir = playerPos - vPos;

		D3DXVec3Normalize(&vDir, &vDir);

		setLook(vDir);
		setPos(vPos);
		setTarget(playerPos);

		ChargeAttack(fDeltaTime, vDir, vPos, playerPos);
	}
}

void CBoss::ChargeAttack(const _float& fDeltaTime, const _vec3 vDir, const _vec3 vPos, const _vec3 vPlayerPos)
{
	m_fSpeed = 20.f;
	_vec3 vLook = vDir;

	D3DXVec3Normalize(&vLook, &vLook);

	m_vFirstPos = vPlayerPos;
	m_vFirstPos += vDir * m_fSpeed * fDeltaTime;

	m_fChargeTime += fDeltaTime;
	if (m_fChargeTime >= 0.6f)
	{
		m_fSpeed = 2.f;
		m_bChargeAttack = false;
		m_fChargeTime = 0.f;
	}
	m_pTransform->setPos(m_vFirstPos);
}

void CBoss::setPos(const _vec3& vPos)
{
	m_vFirstPos = vPos;
}

void CBoss::setLook(const _vec3& vLook)
{
	m_vLook = vLook;
}

void CBoss::setTarget(const _vec3& vTarget)
{
	m_vTargetPos = vTarget;
}

void CBoss::HitBoss(const _float& fTimeDelta)
{
	m_iTimer += fTimeDelta;
	if (m_iTimer >= 2.0f)
	//if (m_iTimer >= 1.0f)
	{
		if (m_pCollision->getHit())
		{
			m_iHP -= 30;
			m_bHPHit = true;
			cout << "Boss got Hit!" << endl;
			cout << m_iHP << endl;

			m_pCollision->Collison(COLLISIONTAG::PLAYER);
			m_pCollision->ResetCollision();
			m_pCollision->setHit(false);

			m_iTimer = 0.f;
		}
	}
}

void CBoss::HitPlayer(const _float& fDeltaTime)
{
	m_iTimer += fDeltaTime;
	if (m_iTimer >= 3.4f)
	{
		cout << "Player got Hit!" << endl;
		m_pAttackColl->setActive(false);
		m_pAttackColl->setHit(false);

		if (m_pAttackColl->getActive())
			m_pAttackColl->Update_Component(fDeltaTime);

		m_pAttackColl->Collison(COLLISIONTAG::PLAYER);

		m_iTimer = 0.f;
	}
}

void CBoss::HPCheck()
{
	if (m_iHP <= 0)
		m_eCurState = STATE::DEATH;

	else if (m_iHP >= 600 && m_iHP <= 1000)
		m_eCurState = STATE::MELEE;

	else if (m_iHP > 1 && m_iHP <= 599)
		m_eCurState = STATE::RANGE;

	//else if (m_iHP > 1 && m_iHP <= 299)
		//m_eCurState = STATE::CHARGE;
	
}

void CBoss::Attack_Dis(const _float& fDeltaTime)
{
	_vec3	m_vInfo;
	m_pTransform->getAxis(VECAXIS::AXIS_POS, m_vInfo);

	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 vPos = pObject->getTransform()->getPos();
	_vec3  vDis = m_vInfo - vPos;
	_float fDis = D3DXVec3Length(&vDis);

	if (m_eCurState == STATE::MELEE)
	{
		if (fDis <= 1.0f)
		{
			Change_State();
			MeleeAttack(fDeltaTime); 
			//
			//m_pAttackColl->setActive(true);
		}
		else if (fDis > 1.0f)
		{
			m_eCurState = STATE::WALKING;
			Change_State();
		}
	}

	if (m_eCurState == STATE::RANGE)
	{
		if (fDis <= 10.0f)
		{
			Change_State();
			RangeAttack(fDeltaTime);
			//m_pAttackColl->setActive(true);
		}
		else if (fDis > 10.0f)
		{
			m_eCurState = STATE::WALKING;
			Change_State();
		}
	}

	if (m_eCurState == STATE::CHARGE)
	{
		if (fDis <= 12.0f)
		{
			Change_State();
			m_fChargeTime += fDeltaTime;
			if (m_fChargeTime >= 1.0f)
			{
				m_bChargeAttack = true;
			}
			ChargeTarget(fDeltaTime); /// targets player and then into chargeattack()
		}
		else if (fDis > 12.0f)
		{
			m_eCurState = STATE::WALKING;
			Change_State();
		}
	}
}

void CBoss::Chase(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3	m_vInfo;
	m_pTransform->getAxis(VECAXIS::AXIS_POS, m_vInfo);

	_matrix matRot;
	matRot = *ComputeLookAtTarget(pTargetPos);

	m_pTransform->setRotate(matRot);

	_vec3 vDir = *pTargetPos - m_vInfo;

	_vec3 vDis = *pTargetPos - m_vInfo;
	_float fDis = D3DXVec3Length(&vDis);

		if (fDis >= 1.0f)
			m_vInfo += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	m_pTransform->setPos(m_vInfo);
}

void CBoss::ChaseRange(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3	m_vInfo;
	m_pTransform->getAxis(VECAXIS::AXIS_POS, m_vInfo);

	_matrix matRot;
	matRot = *ComputeLookAtTarget(pTargetPos);

	m_pTransform->setRotate(matRot);

	_vec3 vDir = *pTargetPos - m_vInfo;

	_vec3 vDis = *pTargetPos - m_vInfo;
	_float fDis = D3DXVec3Length(&vDis);

	if (fDis >= 10.0f)
		m_vInfo += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	m_pTransform->setPos(m_vInfo);
}

void CBoss::ChaseCharge(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3	m_vInfo;
	m_pTransform->getAxis(VECAXIS::AXIS_POS, m_vInfo);

	_matrix matRot;
	matRot = *ComputeLookAtTarget(pTargetPos);

	m_pTransform->setRotate(matRot);

	_vec3 vDir = *pTargetPos - m_vInfo;

	_vec3 vDis = *pTargetPos - m_vInfo;
	_float fDis = D3DXVec3Length(&vDis);

	if (fDis >= 12.0f)
		m_vInfo += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	m_pTransform->setPos(m_vInfo);
}

_matrix* CBoss::ComputeLookAtTarget(const _vec3* pTargetPos)
{
	_matrix matView, matBill;
	D3DXMatrixIdentity(&matBill);

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	return &matBill;
}

CBullet* CBoss::Fireball(GAMEOBJECTID eID)
{
	_vec3 vLook, vPos;
	_float fAngle;
	vPos = m_pTransform->getPos();

	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 playerPos = pObject->getTransform()->getPos();

	m_pTransform->getAxis(VECAXIS::AXIS_LOOK, vLook);
	CGameObject* pBullet = GetGameObject(LAYERID::GAME_LOGIC, eID);

	if (!pBullet)
		pBullet = Clone_ObjProto<CFireball>(eID);

	_vec3 vDir = playerPos - vPos;
	D3DXVec3Normalize(&vDir, &vDir);

	static_cast<CFireball*>(pBullet)->setPos(vPos);
	static_cast<CFireball*>(pBullet)->setLook(vDir);
	static_cast<CFireball*>(pBullet)->setTarget(playerPos);

	return static_cast<CFireball*>(pBullet);
}

void CBoss::Free()
{
	CGameObject::Free();
	Safe_Release(m_pCollision);
	Safe_Release(m_pAttackColl);
	Safe_Release(m_pTexture);
	Safe_Release(m_pAnimator);
	Safe_Release(m_pBufferCom);
}

void CBoss::LoadTransform(const _vec3& vScale, const _vec3& vRotate, const _vec3& vPos)
{
	m_pTransform->setScale(vScale);
	m_pTransform->setAngle(vRotate);
	m_pTransform->setPos(vPos);
}