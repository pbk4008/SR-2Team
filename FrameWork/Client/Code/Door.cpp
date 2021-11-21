#include "pch.h"
#include "Door.h"
#include "Cube.h"
#include "SphereCollision.h"

CDoor::CDoor() : m_pDoorModel(nullptr), m_bEnter(false), m_bClear(false), m_pInteract(nullptr)
, m_fDoorTime(0.f),  m_fDoorSpeed(0.f), m_iIndex(-1)
{
}

CDoor::CDoor(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pDoorModel(nullptr), m_pInteract(nullptr)
,m_bEnter(false), m_bClear(false), m_fDoorTime(0.f), m_fDoorSpeed(0.f), m_iIndex(-1)
{
}

CDoor::CDoor(const CDoor& rhs) : CGameObject(rhs), m_pDoorModel(nullptr), m_bEnter(rhs.m_bEnter),m_bClear(rhs.m_bClear)
, m_fDoorTime(rhs.m_fDoorTime), m_fDoorSpeed(rhs.m_fDoorSpeed), m_iIndex(rhs.m_iIndex),m_pInteract(nullptr)
{
	m_pDoorModel = Clone_ObjProto<CCube>(GAMEOBJECTID::CUBE);
	for (_int i = 0; i < 6; i++)
		m_pDoorModel->setTexture(L"Door", i);
	
}

CDoor::~CDoor()
{
}

HRESULT CDoor::Init_Door()
{
	m_fDoorSpeed = 5.f;
	return S_OK;
}

_int CDoor::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = CGameObject::Update_GameObject(fDeltaTime);
	m_pDoorModel->setTransform(m_pTransform->getScale(), m_pTransform->getAngle(), m_pTransform->getPos());
	if (m_pInteract->getActive())
	{
		m_pInteract->Update_Component(fDeltaTime);
		m_pInteract->Collison(COLLISIONTAG::PLAYER);
	}
	m_pDoorModel->Update_GameObject(fDeltaTime);
	
	DoorOpen(fDeltaTime);
	Insert_RenderGroup(RENDERGROUP::PRIORITY, this);
	return iExit;
}

void CDoor::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
	if (m_pInteract->getHit())
	{
		m_bEnter = true;
		m_pInteract->ResetCollision();
		m_pInteract->setActive(false);
	}
}

void CDoor::Render_GameObject()
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	CGameObject::Render_GameObject();
	if(m_pInteract->getActive())
		m_pInteract->Render_Collision();
}

CGameObject* CDoor::Clone_GameObject()
{
	return new CDoor(*this);
}

void CDoor::ResetObject()
{
}

void CDoor::setCollision()
{
	m_pInteract = Clone_ComProto<CSphereCollision>(COMPONENTID::SPHERECOL);
	m_pInteract->setActive(true);
	m_pInteract->setRadius(3.f);
	m_pInteract->setTransform(m_pTransform);
	m_pInteract->setTag(COLLISIONTAG::ETC);
	m_pInteract->setTrigger(COLLISIONTRIGGER::INTERACT);
	Insert_Collision(m_pInteract);
}

void CDoor::DoorOpen(const _float& fDeltaTime)
{
	_vec3 vPos, vRight;
	vPos=m_pTransform->getPos();
	vRight = { 0.f,0.f,1.f };
	if (m_iIndex == 0)
		vRight = { 1.f,0.f,0.f };
	if (m_iIndex == 1)
		m_fDoorSpeed = -5.f;
	if (m_bEnter)
	{
		m_fDoorTime += fDeltaTime;
		vPos += vRight * -m_fDoorSpeed * fDeltaTime;
		//´ÝÈû
		if (m_fDoorTime > 1.5f)
		{
			m_bEnter = false;
			m_fDoorTime = 0.f;
		}
	}
	if (m_bClear)
	{
		//¿­¸²
		m_fDoorTime += fDeltaTime;
		vPos += vRight * m_fDoorSpeed * fDeltaTime;
		if (m_fDoorTime > 1.5f)
		{
			m_bClear = false;
			m_fDoorTime = 0.f;
		}
	}
	m_pTransform->setPos(vPos);
}

CDoor* CDoor::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CDoor* pInstance = new CDoor(pDevice);
	if (FAILED(pInstance->Init_Door()))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CDoor::Add_Component()
{
	CGameObject::Add_Component();

	return S_OK;
}

void CDoor::Free()
{
	Safe_Release(m_pDoorModel);
	Safe_Release(m_pInteract);
	CGameObject::Free();
}

void CDoor::setTransform(const _vec3& vScale, const _vec3& vRotate, const _vec3& vPos)
{
	m_pTransform->setScale(vScale);
	m_pTransform->setAngle(vRotate);
	m_pTransform->setPos(vPos);
	setCollision();
	m_pDoorModel->LoadTransform(m_pTransform->getScale(), m_pTransform->getAngle(), m_pTransform->getPos());
}

void CDoor::setTrigger(_vec3 vPos)
{
	_vec3 vPivot = vPos - m_pInteract->getCenter();
	m_pInteract->setPivot(vPivot);
}
