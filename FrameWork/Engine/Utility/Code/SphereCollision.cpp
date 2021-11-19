#include "Engine_Include.h"
#include "SphereCollision.h"
#include "CollisionMgr.h"

CSphereCollision::CSphereCollision() : m_pSphere(nullptr), m_fRadius(0.f)
{
}

CSphereCollision::CSphereCollision(LPDIRECT3DDEVICE9 pDevice): CCollision(pDevice), m_pSphere(nullptr), m_fRadius(0.f)
{
}

CSphereCollision::CSphereCollision(const CSphereCollision& rhs): CCollision(rhs), m_pSphere(nullptr), m_fRadius(0.f)
{
	
}

CSphereCollision::~CSphereCollision()
{
}

HRESULT CSphereCollision::Init_SphereCollision()
{
	CCollision::Init_Collision();
	return S_OK;
}

_int CSphereCollision::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;

	iExit = CCollision::Update_Component(fDeltaTime);
	return iExit;
}

void CSphereCollision::Render_Collision()
{
	CCollision::Render_Collision();
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pSphere->DrawSubset(0);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CSphereCollision::Collison(COLLISIONTAG eTag)
{
	m_pCollisionMgr->Collision(this, eTag);
}
CComponent* CSphereCollision::Clone_Component()
{
	return new CSphereCollision(*this);
}

CSphereCollision* CSphereCollision::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSphereCollision* pInstance = new CSphereCollision(pDevice);
	if (FAILED(pInstance->Init_SphereCollision()))
		Safe_Release(pInstance);
	return pInstance;
}

void CSphereCollision::Free()
{
	Safe_Release(m_pSphere);
	CCollision::Free();
}

void CSphereCollision::setRadius(const _float& fRadius)
{
	m_fRadius = fRadius;
	D3DXCreateSphere(m_pDevice, m_fRadius, 5, 5, &m_pSphere, nullptr);
}
