#include "Engine_Include.h"
#include "Collision.h"
#include "Transform.h"
#include "CollisionMgr.h"

CCollision::CCollision() : m_pTransform(nullptr), m_pSphere(nullptr), m_bHit(false),m_eTag(COLLISIONTAG::MAX)
,m_pCollisionMgr(nullptr)
{
	ZeroMemory(&m_vCenter, sizeof(_vec3));
}

CCollision::CCollision(LPDIRECT3DDEVICE9 pDevice) : CComponent(pDevice),m_pTransform(nullptr), m_pSphere(nullptr)
, m_bHit(false), m_eTag(COLLISIONTAG::MAX),m_pCollisionMgr(nullptr)
{
	ZeroMemory(&m_vCenter, sizeof(_vec3));
}

CCollision::CCollision(const CCollision& rhs) : CComponent(rhs), m_vCenter(rhs.m_vCenter), m_pTransform(nullptr), m_fRadius(rhs.m_fRadius)
, m_pSphere(rhs.m_pSphere), m_bHit(rhs.m_bHit),m_pCollisionMgr(rhs.m_pCollisionMgr), m_eTag(rhs.m_eTag)
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
	m_vCenter= *m_pTransform->getAxis(VECAXIS::AXIS_POS);
	return iExit;
}

CComponent* CCollision::Clone_Component()
{
	return new CCollision(*this);
}

void CCollision::Render_Collision()
{
	D3DXCreateSphere(m_pDevice, m_fRadius, 10, 10, &m_pSphere, NULL);
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pSphere->DrawSubset(0);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CCollision::Collison(COLLISIONTAG eTag)
{
	m_pCollisionMgr->Collision(this, eTag);
}

CCollision* CCollision::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCollision* pInstance = new CCollision(pDevice);
	if(FAILED(pInstance->Init_Collision()))
		Safe_Release(pInstance);
	return pInstance;
}

void CCollision::Free()
{
	Safe_Release(m_pSphere);
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

void CCollision::setRadius(const _float& fRadius)
{
	m_fRadius = fRadius;
}

void CCollision::setTransform(CTransform* pTransform)
{
	m_pTransform = pTransform;
	m_pTransform->AddRef();
}

void CCollision::setTag(COLLISIONTAG eTag)
{
	m_eTag = eTag;
}

