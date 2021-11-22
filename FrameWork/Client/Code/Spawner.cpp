#include "pch.h"
#include "Spawner.h"
#include "SpawnerAnim.h"
#include "MeleeMon.h"
#include "ShootMon.h"
#include "FlyMon.h"
#include "Transform.h"
CSpawner::CSpawner() : m_dwMaxMonCount(0), m_dwCurrentMonCount(0), m_pBuffer(nullptr), m_pAnimation(nullptr)
, m_fSpawnTime(0.f),m_dwIndex(0), m_pInteract(nullptr), m_fCollisionTime(0.f)
{
}

CSpawner::CSpawner(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_dwMaxMonCount(0), m_dwCurrentMonCount(0)
,m_pBuffer(nullptr), m_pAnimation(nullptr), m_fSpawnTime(0.f), m_dwIndex(0), m_pInteract(nullptr)
, m_fCollisionTime(0.f)
{
}

CSpawner::CSpawner(const CSpawner& rhs) : CGameObject(rhs), m_dwMaxMonCount(rhs.m_dwMaxMonCount), m_dwCurrentMonCount(rhs.m_dwCurrentMonCount)
, m_pBuffer(rhs.m_pBuffer), m_pAnimation(nullptr), m_fSpawnTime(rhs.m_fSpawnTime), m_dwIndex(rhs.m_dwIndex)
, m_pInteract(nullptr), m_fCollisionTime(rhs.m_fCollisionTime)
{
	m_pBuffer->AddRef();

	SettingAnimation();
	
}

CSpawner::~CSpawner()
{
}

HRESULT CSpawner::Init_Spawner()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_dwMaxMonCount = 30.f;
	m_dwCurrentMonCount = m_dwMaxMonCount;
	return S_OK;
}

_int CSpawner::Update_GameObject(const _float& fDeltaTime)
{
	m_pTransform->setAngle(m_pTransform->getAngle());
	m_pTransform->setPos(m_pTransform->getPos());

	_int iExit = CGameObject::Update_GameObject(fDeltaTime);
	if (m_pInteract)
		m_pInteract->Update_Component(fDeltaTime);
	SpawnMonster(fDeltaTime);
	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);
	return iExit;
}

void CSpawner::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
	if (m_pInteract)
	{
		if (m_pInteract->getHit())
		{
			m_fCollisionTime += GetOutDeltaTime();
			if (m_fCollisionTime > 2.f)
			{
				m_fCollisionTime = 0.f;
				setActive(false);
			}
		}
		else
		{
			m_fCollisionTime = 0.f;
			m_pInteract->ResetCollision();
		}
	}
}

void CSpawner::Render_GameObject()
{
	CGameObject::Render_GameObject();
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	m_pAnimation->Render_Animation();
	m_pBuffer->Render_Buffer();
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	
}

CGameObject* CSpawner::Clone_GameObject()
{
	return new CSpawner(*this);
}

void CSpawner::ResetObject()
{
}

void CSpawner::SettingAnimation()
{
	m_pAnimation = CSpawnerAnim::Create(m_pDevice);
	m_pAnimation->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::ANIMATION, m_pAnimation);
}

void CSpawner::SettingCollision()
{
	m_pInteract = Clone_ComProto<CSphereCollision>(COMPONENTID::SPHERECOL);
	m_pInteract->setRadius(2.f);
	m_pInteract->setActive(true);
	m_pInteract->setTag(COLLISIONTAG::ETC);
	m_pInteract->setTrigger(COLLISIONTRIGGER::INTERACT);
	m_pInteract->setTransform(m_pTransform);
	Insert_Collision(m_pInteract);
}

void CSpawner::SpawnMonster(const _float& fDeltaTime)
{
	if (m_dwCurrentMonCount > 0)
	{
		m_fSpawnTime += fDeltaTime;

		if (m_dwIndex == 1)
			m_dwCurrentMonCount--;
		if (m_fSpawnTime > 1.5f)
		{
			RandomMonSpawn();
			m_fSpawnTime = 0.f;
		}
	}
	else
		setActive(false);
}

void CSpawner::RandomMonSpawn()
{
	CGameObject* pMonster = nullptr;
	_int randNum = rand() % 3;
	_vec3 vPos = m_pTransform->getPos();
	switch (randNum)
	{
	case 0:
		pMonster = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1);
		if (!pMonster)
		{
			pMonster = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
			Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMonster);
		}
		break;
	case 1:
		pMonster = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2);
		if (!pMonster)
		{
			pMonster = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
			Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2, pMonster);
		}
		break;
	case 2:
		pMonster = GetGameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER3);
		if (!pMonster)
		{
			pMonster = Clone_ObjProto<CFlyMon>(GAMEOBJECTID::MONSTER3);
			Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER3, pMonster);
		}
		break;
	}
	if (m_dwIndex == 0)
		static_cast<CMonster*>(pMonster)->setItemCheck(true);
	pMonster->getTransform()->setPos(vPos);
}

CSpawner* CSpawner::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSpawner* pInstance = new CSpawner(pDevice);
	if (FAILED(pInstance->Init_Spawner()))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CSpawner::Add_Component()
{
	CGameObject::Add_Component();

	CComponent* pCom = nullptr;
	pCom = m_pBuffer = Clone_ComProto<CRcTex>(COMPONENTID::RCTEX);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_pBuffer->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_STATIC].emplace(COMPONENTID::RCTEX, pCom);

	return S_OK;
}

void CSpawner::Free()
{
	CGameObject::Free();
	Safe_Release(m_pBuffer);
	Safe_Release(m_pAnimation);
	Safe_Release(m_pInteract);
}

void CSpawner::setSettingSpawner(const _ulong& dwMaxCount)
{
	m_dwMaxMonCount = dwMaxCount;
	m_dwCurrentMonCount = m_dwMaxMonCount;
}

void CSpawner::setTransform(const _vec3& vPos, const _vec3& vAngle)
{
	m_pTransform->setAngle(vAngle);
	m_pTransform->setPos(vPos);
}
