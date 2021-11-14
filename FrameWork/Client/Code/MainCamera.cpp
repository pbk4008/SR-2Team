#include "pch.h"
#include "MainCamera.h"
#include "Camera.h"

CMainCamera::CMainCamera() : m_pCamera(nullptr) ,m_iRoutin(0), m_fHitDelay(0.f), m_bLRCheck(false)
{
}

CMainCamera::CMainCamera(LPDIRECT3DDEVICE9 pDevice) : CGameObject(pDevice), m_pCamera(nullptr), m_iRoutin(0), m_fHitDelay(0.f), m_bLRCheck(false)
{

}

CMainCamera::CMainCamera(const CMainCamera& rhs) : CGameObject(rhs), m_pCamera(rhs.m_pCamera), m_iRoutin(rhs.m_iRoutin), m_fHitDelay(rhs.m_fHitDelay), m_bLRCheck(rhs.m_bLRCheck)
{
	m_pCamera->AddRef();
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
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->getWorldMatrix());
	CGameObject::Render_GameObject();
}

CMainCamera* CMainCamera::Clone_GameObject()
{
	return new CMainCamera(*this);
}

CPlayer::STATE CMainCamera::Hit()
{
	_float fDeltaTime = GetOutDeltaTime();
	_vec3 vEye,vAt,vRight;
	vEye=m_pCamera->getEye();
	vAt = m_pCamera->getAt();
	m_pTransform->getAxis(VECAXIS::AXIS_RIGHT, vRight);
	D3DXVec3Normalize(&vRight, &vRight);
	if (m_bLRCheck)
	{
		vEye += vRight * 10.f * fDeltaTime;
		vAt += vRight * 10.f * fDeltaTime;
	}
	else
	{
		vEye -= vRight * 10.f * fDeltaTime;
		vAt -= vRight * 10.f * fDeltaTime;
	}
	m_pCamera->setAt(vAt);
	m_pCamera->setEye(vEye);
	m_fHitDelay += fDeltaTime;

	if (m_fHitDelay > 0.1f)
	{
		m_bLRCheck = !m_bLRCheck;
		m_fHitDelay = 0.f;
		m_iRoutin++;
	}
	if (m_iRoutin > 3)
	{
		vEye = m_pTransform->getPos();
		_vec3 vLook;
		m_pTransform->getAxis(VECAXIS::AXIS_LOOK, vLook);

		D3DXVec3Normalize(&vLook, &vLook);
		vAt = vEye + vLook;
		m_pCamera->setAt(vAt);
		m_pCamera->setEye(vEye);
		m_iRoutin = 0;
		return CPlayer::STATE::IDLE;
	}
	else
		return CPlayer::STATE::HIT;

}

void CMainCamera::CameraZoomInAndOut(const _float& fDeltaTime)
{
	m_pCamera->ZoomInAndOut(fDeltaTime);
}

void CMainCamera::CameraZoomReset()
{
	m_pCamera->ResetZoom();
}

void CMainCamera::FollowTarget()
{
	//m_pTransform->setPos(-0.8f, 0.5f, -1.75f);

	//m_pTransform->setPos(0.f, 0.f, -3.f);//플레이어 디버깅용 카메라
	m_pTransform->setPos(-0.7f, 3.f, -10.f);//몬스터 디버깅용 카메라
	_vec3 vEye, vLook;
	m_pTransform->getAxis(VECAXIS::AXIS_POS, vEye);
	m_pTransform->getAxis(VECAXIS::AXIS_LOOK,vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vAt = vEye + vLook;

	m_pCamera->setEye(vEye);
	m_pCamera->setAt(vAt);
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
	m_pCamera->AddRef();
	m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_DYNAMIC].emplace(COMPONENTID::CAMERA, pCom);

	return S_OK;
}

void CMainCamera::Free()
{
	Safe_Release(m_pCamera);
	CGameObject::Free();
}

void CMainCamera::setTarget(CTransform* pTarget)
{
	m_pTransform->setParent(pTarget);
}
