#include "pch.h"
#include "StaticCamera.h"


CStaticCamera::CStaticCamera()
{

}
CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
{

}


CStaticCamera::~CStaticCamera(void)
{

}

HRESULT CStaticCamera::Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	//받은 정보를 바탕으로 카메라 생성
	CCamera::Init_Camera();


	return S_OK;
}

Engine::_int CStaticCamera::Update_Object(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);
	Target_Renewal();

	return CCamera::Update_Component(fTimeDelta);
}

void CStaticCamera::Key_Input(const _float& fTimeDelta)
{
	if (Key_Pressing(VIR_W)) //가까워진다.
	{
		m_fDistance -= fTimeDelta * m_fSpeed;
	}
	if (Key_Pressing(VIR_S)) // 멀어진다.
	{
		m_fDistance += fTimeDelta * m_fSpeed;
	}
	if (Key_Pressing(VIR_A))
	{
		m_fAngle += D3DXToRadian(90.f) * fTimeDelta;
	}
	if (Key_Pressing(VIR_D))
	{
		m_fAngle -= D3DXToRadian(90.f) * fTimeDelta;
	}


}

void CStaticCamera::Target_Renewal(void)
{
	
	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Get_Component(LAYERID::GAME_LOGIC, GAMEOBJECTID::PLAYER, COMPONENTID::TRANSFORM, COMPONENTTYPE::TYPE_DYNAMIC););
	NULL_CHECK(pPlayerTransform);

	_vec3	vLook;
	_matrix mMatrix = pPlayerTransform->getWorldMatrix();
	memcpy(&vLook, &mMatrix.m[2][0], sizeof(_vec3));

	m_vEye = vLook * -1.f;		// 위치? 방향? 방향
	D3DXVec3Normalize(&m_vEye, &m_vEye);

	m_vEye *= m_fDistance;	// 위치? 방향? 방향

	_vec3	vRight;
	memcpy(&vRight, &pPlayerTransform->getWorldMatrix().m[0][0], sizeof(_vec3));

	D3DXMATRIX	matRot;
	D3DXMatrixRotationAxis(&matRot, &vRight, m_fAngle);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRot);// 위치? 방향? 방향

	_vec3


	m_vEye += pPlayerTransform->getWorldMatrix().m[2][0] // 위치
	m_vAt = pPlayerTransform->m_vInfo[INFO_POS];
}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CStaticCamera* pInstance = new CStaticCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
		Safe_Release(pInstance);

	return pInstance;


}