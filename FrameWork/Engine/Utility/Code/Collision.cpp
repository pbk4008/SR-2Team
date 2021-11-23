#include "Engine_Include.h"
#include "Collision.h"
#include "Transform.h"
#include "CollisionMgr.h"
#include "Export_Function.h"

CCollision::CCollision() : m_pTransform(nullptr), m_bHit(false),m_eTag(COLLISIONTAG::MAX)
,m_pCollisionMgr(nullptr), m_eTrigger(COLLISIONTRIGGER::MAX), m_pCollider(nullptr) 
, m_pTarget(nullptr), m_fPivotLen(0.f)
{
	ZeroMemory(&m_vCenter, sizeof(_vec3));
	ZeroMemory(&m_vPivot, sizeof(_vec3));
}

CCollision::CCollision(LPDIRECT3DDEVICE9 pDevice) : CComponent(pDevice),m_pTransform(nullptr), 
 m_bHit(false), m_eTag(COLLISIONTAG::MAX),m_pCollisionMgr(nullptr), m_eTrigger(COLLISIONTRIGGER::MAX), m_pCollider(nullptr)
, m_pTarget(nullptr), m_fPivotLen(0.f)
{
	ZeroMemory(&m_vCenter, sizeof(_vec3));
	ZeroMemory(&m_vPivot, sizeof(_vec3));
}

CCollision::CCollision(const CCollision& rhs) : CComponent(rhs), m_vCenter(rhs.m_vCenter), m_pTransform(nullptr)
, m_bHit(rhs.m_bHit),m_pCollisionMgr(rhs.m_pCollisionMgr), m_eTag(rhs.m_eTag), m_pCollider(rhs.m_pCollider)
, m_eTrigger(rhs.m_eTrigger),m_vPivot(rhs.m_vPivot), m_pTarget(rhs.m_pTarget), m_fPivotLen(rhs.m_fPivotLen)
{
	if(rhs.m_pTarget)
		m_pTarget->AddRef();
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
	if (m_fPivotLen>0)
	{
		_vec3 vLook;
		m_pTransform->getAxis(VECAXIS::AXIS_LOOK, vLook);
		vLook *= m_fPivotLen;
		m_vCenter += vLook;
	}
	else
		m_vCenter += m_vPivot;
	return iExit;
}

void CCollision::Render_Collision()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
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
_bool CCollision::WallCollision()
{
	_vec3 vMove;
	ZeroMemory(&vMove, sizeof(_vec3));
	ZeroMemory(&mvecWalkPower, sizeof(_vec3));
	_vec3 vecWalkPower;
	m_pTarget->GetWalkPower(&vecWalkPower);

	m_pCollisionMgr->WallCollision(this, vecWalkPower,&mvecWalkPower);

	_vec3 vPos=m_pTarget->getTransform()->getPos();
	vPos += mvecWalkPower;

	vPos += vMove;
	
	m_pTarget->getTransform()->setPos(vPos);
	if (m_pTransform->IsZero(vMove))
		return false;
	return true;
}
void CCollision::Free()
{
	CComponent::Free();
	Safe_Release(m_pCollisionMgr);
	Safe_Release(m_pTransform);
}

COLLISIONTAG CCollision::getTag()
{
	return m_eTag;
}


void CCollision::setTransform(const _vec3& pCenter)
{
	m_vCenter = pCenter;
	m_pTransform = Clone_ComProto<CTransform>(COMPONENTID::TRANSFORM);
	m_pTransform->setPos(pCenter);
}

void CCollision::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
	m_vCenter = m_pTransform->getPos();
}

void CCollision::setTag(COLLISIONTAG eTag)
{
	m_eTag = eTag;
}
void CCollision::setTarget(CGameObject* pTarget)
{
	m_pTarget = pTarget; 
}

