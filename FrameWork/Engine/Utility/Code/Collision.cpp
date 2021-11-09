#include "Engine_Include.h"
#include "Collision.h"
#include "Transform.h"

CCollision::CCollision() : m_pTransform(nullptr)
{
	ZeroMemory(&m_vCenter, sizeof(_vec3));
}

CCollision::CCollision(LPDIRECT3DDEVICE9 pDevice) : CComponent(pDevice),m_pTransform(nullptr)
{
	ZeroMemory(&m_vCenter, sizeof(_vec3));
}

CCollision::CCollision(const CCollision& rhs) : CComponent(rhs), m_vCenter(rhs.m_vCenter), m_pTransform(rhs.m_pTransform)
{
	if (rhs.m_pTransform)
		m_pTransform->AddRef();
}

CCollision::~CCollision()
{
}

HRESULT CCollision::Init_Collision()
{
	//m_bActive = false;

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
	LPD3DXMESH pSphere = nullptr;
	D3DXCreateSphere(m_pDevice, m_fRadius, 10, 10, &pSphere, NULL);
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	pSphere->DrawSubset(0);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
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
	Safe_Release(m_pTransform);
	CComponent::Free();
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
