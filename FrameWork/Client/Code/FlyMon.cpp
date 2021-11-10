#include "pch.h"
#include "FlyMon.h"
#include "Transform.h"
#include "Player.h"
#include "FlyMon_Attack.h"
#include "FlyMon_Idle.h"
#include "FlyMon_WalkF.h"

CFlyMon::CFlyMon()
	: m_pBufferCom(nullptr), m_pTexture(nullptr), m_fSpeed(0.f),
	m_bAttack(false), m_iTimer(1), m_pAnimator(nullptr),
	m_eCurState(STATE::MAX), m_ePreState(STATE::MAX), m_bMoving(false),
	m_pCollision(nullptr), m_pAttackColl(nullptr)
{

}

CFlyMon::CFlyMon(LPDIRECT3DDEVICE9 pDevice)
	: CMonster(pDevice), m_pBufferCom(nullptr), m_pTexture(nullptr),
	m_fSpeed(0.f), m_bAttack(false), m_iTimer(1),
	m_pAnimator(nullptr), m_eCurState(STATE::MAX), m_ePreState(STATE::MAX),
	m_bMoving(false), m_pCollision(nullptr), m_pAttackColl(nullptr)
{

}

CFlyMon::CFlyMon(const CFlyMon& rhs)
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

	_vec3	m_vInfo;
	m_pTransform->getAxis(VECAXIS::AXIS_POS, m_vInfo);

	Follow(fDeltaTime);

	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 vPos = pObject->getTransform()->getPos();

	_vec3  vDis = m_vInfo - vPos;

	_float fDis = D3DXVec3Length(&vDis);

	if (fDis <= 1.0f)
	{
		Attack(fDeltaTime);
	}

	iExit = CGameObject::Update_GameObject(fDeltaTime);
	Insert_RenderGroup(RENDERGROUP::ALPHA, this);

	return iExit;
}

void CFlyMon::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();

}

void CFlyMon::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());

	m_pTexture->Render_Texture();
	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

CGameObject* CFlyMon::Clone_GameObject()
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
		}
		m_ePreState = m_eCurState;
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

	//texture
	pComponent = m_pTexture = Clone_ComProto<CTexture>(COMPONENTID::FLYMON_TEX);
	m_pTexture->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::FLYMON_TEX, pComponent);

	return S_OK;
}

void CFlyMon::Follow(const _float& fDeltaTime)
{
	CGameObject* pObject = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER);
	_vec3 playerPos = pObject->getTransform()->getPos();

	Chase_Target(&playerPos, m_fSpeed, fDeltaTime);
}


void CFlyMon::Attack(const _float& fDeltaTime)
{
	m_iTimer += fDeltaTime;
	if (m_iTimer >= 1.0f)
	{
		cout << "Fly!" << endl;
		m_bAttack = false;
		m_iTimer = 0.f;
	}
}

void CFlyMon::Free()
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pBufferCom);
	CGameObject::Free();
}
