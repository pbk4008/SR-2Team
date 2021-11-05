#include "pch.h"
#include "MainCamera.h"
#include "Camera.h"

CMainCamera::CMainCamera() : m_pCamera(nullptr), m_pTarget(nullptr)
{
}

CMainCamera::CMainCamera(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pCamera(nullptr), m_pTarget(nullptr)
{

}

CMainCamera::CMainCamera(const CMainCamera& rhs) : CGameObject(rhs), m_pCamera(rhs.m_pCamera), m_pTarget(rhs.m_pTarget)
{
	m_pCamera->AddRef();
	if(rhs.m_pTarget)
		m_pTarget->AddRef();
}

CMainCamera::~CMainCamera()
{
}

HRESULT CMainCamera::Init_MainCamera()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CMainCamera::Update_GameObject(const _float& fDeltaTime)
{
	_int iExit = 0;

	iExit = CGameObject::Update_GameObject(fDeltaTime);
	FollowTarget();

	return iExit;
}

void CMainCamera::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CMainCamera::Render_GameObject()
{
	CGameObject::Render_GameObject();
}

CMainCamera* CMainCamera::Clone_GameObject()
{
	return new CMainCamera(*this);
}

void CMainCamera::FollowTarget()
{
	//_matrix matWorld = m_pTransform->getWorldMatrix();
	_matrix matWorld = m_pTarget->getTransform()->getWorldMatrix();

	_vec3 vEye;
	memcpy(&vEye, &matWorld.m[2][0], sizeof(_vec3));

	vEye *= -1.f;
	D3DXVec3Normalize(&vEye, &vEye);

	vEye *= 10.f;

	_vec3 vRight;
	memcpy(&vRight, &matWorld.m[0][0], sizeof(_vec3));

	_matrix matRot;
	D3DXMatrixRotationAxis(&matRot, &vRight, 0.f);
	D3DXVec3TransformNormal(&vEye, &vEye, &matRot);

	_vec3 TargetPos;
	memcpy(&TargetPos, &matWorld.m[3][0], sizeof(_vec3));
	m_pCamera->setEye(vEye + TargetPos);
	m_pCamera->setAt(TargetPos);

	/*_vec3 vAt = m_pCamera->getAt();
	if (m_pTarget->getMove())
	{
		D3DXVec3TransformCoord(&vEye, &vEye, &matWorld);
		D3DXVec3TransformCoord(&vAt, &vAt, &matWorld);

		m_pCamera->setEye(vEye);
		m_pCamera->setAt(vAt);
	}*/
}

CMainCamera* CMainCamera::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMainCamera* pInstance = new CMainCamera(pDevice);
	if (FAILED(pInstance->Init_MainCamera()))
		Safe_Release(pInstance);
	return pInstance;
}

HRESULT CMainCamera::Add_Component()
{
	CGameObject::Add_Component();

	CComponent* pCom = nullptr;

	pCom = m_pCamera = Clone_ComProto<CCamera>(COMPONENTID::CAMERA);
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::CAMERA, pCom);

	return S_OK;
}

void CMainCamera::Free()
{
	Safe_Release(m_pTarget);
	Safe_Release(m_pCamera);
	CGameObject::Free();
}

void CMainCamera::setTarget(CGameObject* pTarget)
{
	m_pTarget = pTarget;
	m_pTransform->setParent(pTarget->getTransform());
}
