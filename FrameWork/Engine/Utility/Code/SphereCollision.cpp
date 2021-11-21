#include "Engine_Include.h"
#include "SphereCollision.h"
#include "CollisionMgr.h"
#include "Transform.h"

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

	m_pTransform->getAxis(VECAXIS::AXIS_POS, m_vCenter);
	iExit = CCollision::Update_Component(fDeltaTime);
	return iExit;
}

void CSphereCollision::Render_Collision()
{
	_matrix matWorld = m_pTransform->getWorldMatrix();
	_vec3 vAxisX, vAxisY, vAxisZ;
	memcpy(vAxisX, &matWorld.m[0], sizeof(_vec3));
	memcpy(vAxisY, &matWorld.m[1], sizeof(_vec3));
	memcpy(vAxisZ, &matWorld.m[2], sizeof(_vec3));

	D3DXVec3Normalize(&vAxisX, &vAxisX);
	D3DXVec3Normalize(&vAxisY, &vAxisY);
	D3DXVec3Normalize(&vAxisZ, &vAxisZ);

	memcpy(&matWorld.m[0], vAxisX, sizeof(_vec3));
	memcpy(&matWorld.m[1], vAxisY, sizeof(_vec3));
	memcpy(&matWorld.m[2], vAxisZ, sizeof(_vec3));

	matWorld.m[3][0] = m_vCenter.x;
	matWorld.m[3][1] = m_vCenter.y;
	matWorld.m[3][2] = m_vCenter.z;
	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
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
	D3DXCreateSphere(m_pDevice, m_fRadius, 10, 10, &m_pSphere, nullptr);
}
