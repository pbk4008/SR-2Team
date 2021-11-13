#include "Engine_Include.h"
#include "Camera.h"


CCamera::CCamera() : m_fAspect(0.f), m_fFar(0.f), m_fFov(0.f), m_fNear(0.f),m_bProjection(false)
, m_ZoomTime(0.f), m_fCurFov(0.f)
{
	ZeroMemory(&m_vEye,sizeof(_vec3));
	ZeroMemory(&m_vAt,sizeof(_vec3));
	ZeroMemory(&m_vUp,sizeof(_vec3));
	ZeroMemory(&m_matView,sizeof(_matrix));
	ZeroMemory(&m_matProjection,sizeof(_matrix));
}

CCamera::CCamera(LPDIRECT3DDEVICE9 pDevice) : CComponent(pDevice), m_fAspect(0.f), m_fFar(0.f), m_fFov(0.f), m_fNear(0.f)
,m_bProjection(false), m_ZoomTime(0.f), m_fCurFov(0.f)

{
	ZeroMemory(&m_vEye, sizeof(_vec3));
	ZeroMemory(&m_vAt, sizeof(_vec3));
	ZeroMemory(&m_vUp, sizeof(_vec3));
	ZeroMemory(&m_matView, sizeof(_matrix));
	ZeroMemory(&m_matProjection, sizeof(_matrix));
}
CCamera::CCamera(const CCamera& rhs) : CComponent(rhs), m_fAspect(rhs.m_fAspect), m_fFar(rhs.m_fFar), m_fFov(rhs.m_fFov), m_fNear(rhs.m_fNear)
, m_vEye(rhs.m_vEye), m_vAt(rhs.m_vAt), m_vUp(rhs.m_vUp), m_bProjection(rhs.m_bProjection)
, m_matView(rhs.m_matView), m_matProjection(rhs.m_matProjection), m_ZoomTime(rhs.m_ZoomTime), m_fCurFov(rhs.m_fCurFov)
{

}


CCamera::~CCamera()
{
}

HRESULT CCamera::Init_Camera(const _vec3& pEye, const _vec3& pAt, const _vec3& pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	m_vEye = pEye;
	m_vAt = pAt;
	m_vUp = pUp;
	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;
	m_fCurFov = m_fFov;

	D3DXMatrixPerspectiveFovLH(&m_matProjection, m_fFov, m_fAspect, m_fNear, m_fFar);

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	return S_OK;
}

HRESULT CCamera::Init_Camera()
{
	D3DXMatrixPerspectiveFovLH(&m_matProjection, m_fFov, m_fAspect, m_fNear, m_fFar);

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	return S_OK;
}

_int CCamera::Update_Component(const _float& fDeltaTime)
{
	if (!m_bProjection)
	{
		m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProjection);
		m_bProjection = true;
	}
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);

	return 0;
}

CComponent* CCamera::Clone_Component()
{
	return new CCamera(*this);
}

void CCamera::ZoomInAndOut(const _float& fDeltaTime)
{
	m_ZoomTime += fDeltaTime;
	m_fFov -= 0.02f;
	if (m_ZoomTime > 0.3f)
	{
		m_fFov = m_fCurFov;
		m_ZoomTime = 0.f;
	}
	D3DXMatrixPerspectiveFovLH(&m_matProjection, m_fFov, m_fAspect, m_fNear, m_fFar);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProjection);
}

void CCamera::ResetZoom()
{
	m_fFov = m_fCurFov;
	m_ZoomTime = 0.f;
	D3DXMatrixPerspectiveFovLH(&m_matProjection, m_fFov, m_fAspect, m_fNear, m_fFar);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProjection);
}

void CCamera::Free()
{
	CComponent::Free();
}

CCamera* CCamera::Create(LPDIRECT3DDEVICE9 pDevice, const _vec3& pEye, const _vec3& pAt, const _vec3& pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	
		CCamera* pInstance = new CCamera(pDevice);
		if (FAILED(pInstance->Init_Camera(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
			Safe_Release(pInstance);
		return pInstance;
	
}

