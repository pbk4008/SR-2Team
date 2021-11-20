#include "Engine_Include.h"
#include "Collision.h"
#include "Transform.h"
#include "CollisionMgr.h"

CCollision::CCollision() : m_pTransform(nullptr), m_bHit(false),m_eTag(COLLISIONTAG::MAX)
,m_pCollisionMgr(nullptr), m_eTrigger(COLLISIONTRIGGER::MAX), m_pCollider(nullptr), m_fPivotLen(0.f)
{
	ZeroMemory(&m_vCenter, sizeof(_vec3));
}

CCollision::CCollision(LPDIRECT3DDEVICE9 pDevice) : CComponent(pDevice),m_pTransform(nullptr), 
 m_bHit(false), m_eTag(COLLISIONTAG::MAX),m_pCollisionMgr(nullptr), m_eTrigger(COLLISIONTRIGGER::MAX), m_pCollider(nullptr)
,m_fPivotLen(0.f)
{
	ZeroMemory(&m_vCenter, sizeof(_vec3));
}

CCollision::CCollision(const CCollision& rhs) : CComponent(rhs), m_vCenter(rhs.m_vCenter), m_pTransform(nullptr)
, m_bHit(rhs.m_bHit),m_pCollisionMgr(rhs.m_pCollisionMgr), m_eTag(rhs.m_eTag), m_pCollider(rhs.m_pCollider)
, m_eTrigger(rhs.m_eTrigger),m_fPivotLen(rhs.m_fPivotLen)
{
	m_pCollisionMgr->AddRef();
	if (rhs.m_pTransform)
		m_pTransform->AddRef();
}

CCollision::~CCollision()
{
}

HRESULT CCollision::Init_Collision()
{
	m_pCollisionMgr = CCollisionMgr::GetInstance();
	NULL_CHECK_RETURN(m_pCollisionMgr, E_FAIL);
	m_pCollisionMgr->AddRef();
	m_bActive = false;

	return S_OK;
}

_int CCollision::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CComponent::Update_Component(fDeltaTime);
	m_pTransform->getAxis(VECAXIS::AXIS_POS,m_vCenter);
	_vec3 vLook;
	m_pTransform->getAxis(VECAXIS::AXIS_LOOK, vLook);
	vLook *= m_fPivotLen;
	m_vCenter += vLook;
	return iExit;
}

void CCollision::Render_Collision()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CCollision::Collison(COLLISIONTAG eTag)
{
}

void CCollision::ResetCollision()
{
	m_bHit = false;
	m_pCollider = nullptr;
}
void CCollision::WallCollision()
{
	_vec3 vMove;
	ZeroMemory(&vMove, sizeof(_vec3));
	m_pCollisionMgr->WallCollision(this, vMove);
	_vec3 vPos=m_pTransform->getPos();

	vPos += vMove;

	m_pTransform->setPos(vPos);
}
void CCollision::Free()
{
	Safe_Release(m_pCollisionMgr);
	Safe_Release(m_pTransform);
	CComponent::Free();
}

COLLISIONTAG CCollision::getTag()
{
	return m_eTag;
}

void CCollision::setCenter(const _vec3& pCenter)
{
	m_vCenter = pCenter;
}


void CCollision::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_vCenter = m_pTransform->getPos();
	m_pTransform->AddRef();
}

void CCollision::setTag(COLLISIONTAG eTag)
{
	m_eTag = eTag;
}

