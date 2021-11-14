#include "pch.h"
#include "Boss.h"
#include "Boss_IdleAnim.h"
#include "Boss_WalkAnim.h"
#include "Boss_MeleeAnim.h"
#include "Boss_RangeAnim.h"
#include "Boss_DeathAnim.h"
#include "Boss_FlyAnim.h"

CBoss::CBoss()
	: m_pBufferCom(nullptr), m_pTexture(nullptr), m_pAnimator(nullptr),
	m_eCurState(STATE::MAX), m_ePreState(STATE::MAX), m_pCollision(nullptr), m_pAttackColl(nullptr),
	m_iHP(0), m_bAttack(false), m_bMoving(false), m_iTimer(0), m_fSpeed(0.f)
{

}

CBoss::CBoss(LPDIRECT3DDEVICE9 pDevice)
	: CGameObject(pDevice), m_pBufferCom(nullptr), m_pTexture(nullptr), m_pAnimator(nullptr),
	m_eCurState(STATE::MAX), m_ePreState(STATE::MAX), m_pCollision(nullptr), m_pAttackColl(nullptr),
	m_iHP(0), m_bAttack(false), m_bMoving(false), m_iTimer(0), m_fSpeed(0.f)
{

}

CBoss::CBoss(const CBoss& rhs)
	: CGameObject(rhs), m_pBufferCom(nullptr), m_pTexture(nullptr),
	m_fSpeed(0.f), m_bAttack(false), m_iTimer(1),
	m_pAnimator(nullptr), m_eCurState(STATE::MAX), m_ePreState(STATE::MAX),
	m_bMoving(false), m_pCollision(nullptr), m_pAttackColl(nullptr), m_iHP(0)
{
}

CBoss::~CBoss()
{

}

HRESULT CBoss::Init_Boss()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 2.f;

	return S_OK;
}

Engine::_int CBoss::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;
	m_pTransform->UsingGravity(fDeltaTime);

	iExit = CGameObject::Update_GameObject(fDeltaTime);
	Insert_RenderGroup(RENDERGROUP::ALPHA, this);

	return iExit;
}

void CBoss::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();

}

void CBoss::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	m_pCollision->Render_Collision();

	m_pAnimator->Render_Animator();
	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

Engine::CGameObject* CBoss::Clone_GameObject()
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

	CBoss_FlyAnim* pFly = CBoss_FlyAnim::Create(m_pDevice);
	m_pAnimator->Insert_Animation(L"Boss_Fly", L"Boss_Idle", pFly, true);


	// connect
	/*m_pAnimator->Connet_Animation(L"Boss_WalkF", L"Boss_Attack");
	m_pAnimator->Connet_Animation(L"Boss_Attack", L"Boss_WalkF");
	m_pAnimator->Connet_Animation(L"Boss_WalkF", L"Boss_Death");
	m_pAnimator->Connet_Animation(L"Boss_Attack", L"Boss_Death");*/

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

	// collision
	m_pCollision = Clone_ComProto<CCollision>(COMPONENTID::COLLISION);
	m_pCollision->setRadius(1.f);
	m_pCollision->setTag(COLLISIONTAG::MONSTER);
	m_pCollision->setActive(true);
	m_pCollision->setTrigger(COLLISIONTRIGGER::HIT);
	m_pCollision->setTransform(m_pTransform);
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

void CBoss::Follow(const _float& fDeltaTime)
{

}

void CBoss::Attack(const _float& fDeltaTime)
{

}

void CBoss::Attack_Dis(const _float& fDeltaTime)
{

}

void CBoss::Free()
{
	Safe_Release(m_pCollision);
	Safe_Release(m_pAttackColl);
	ClearCollisionList();
	Safe_Release(m_pTexture);
	Safe_Release(m_pAnimator);
	Safe_Release(m_pBufferCom);
	CGameObject::Free();
}
