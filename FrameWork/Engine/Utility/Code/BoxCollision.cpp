#include "Engine_Include.h"
#include "BoxCollision.h"
#include "CollisionMgr.h"
#include "Transform.h"
CBoxCollision::CBoxCollision(): m_pBox(nullptr)
{
	ZeroMemory(&m_vScale, sizeof(_vec3));
}

CBoxCollision::CBoxCollision(LPDIRECT3DDEVICE9 pDevice) : CCollision(pDevice), m_pBox(nullptr)
{
	ZeroMemory(&m_vScale, sizeof(_vec3));
}

CBoxCollision::CBoxCollision(const CBoxCollision& rhs) : CCollision(rhs), m_vScale(rhs.m_vScale), m_pBox(nullptr)
{
}

CBoxCollision::~CBoxCollision()
{
}

HRESULT CBoxCollision::Init_BoxCollision()
{
	CCollision::Init_Collision();
	return S_OK;
}

_int CBoxCollision::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CCollision::Update_Component(fDeltaTime);
	return iExit;
}

void CBoxCollision::Render_Collision()
{
	CCollision::Render_Collision();
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pBox->DrawSubset(0);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CBoxCollision::Collison(COLLISIONTAG eTag)
{
	m_pCollisionMgr->Collision(this,eTag);
}

CComponent* CBoxCollision::Clone_Component()
{
	return new CBoxCollision(*this);
}

CBoxCollision* CBoxCollision::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBoxCollision* pInstance = new CBoxCollision(pDevice);
	if (FAILED(pInstance->Init_BoxCollision()))
		Safe_Release(pInstance);
	return pInstance;
}
void CBoxCollision::Free()
{
	CCollision::Free();
}

void CBoxCollision::setAxis(const _vec3& vAxis,const _vec3& vRotate)
{
	m_vScale = vAxis;
	m_vAngle = vRotate;
	D3DXCreateBox(m_pDevice, 1.f, 1.f, 1.f, &m_pBox, nullptr);
}
