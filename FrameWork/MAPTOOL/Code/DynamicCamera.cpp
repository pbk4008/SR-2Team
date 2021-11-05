#include "pch.h"
#include "DynamicCamera.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{

}

CDynamicCamera::~CDynamicCamera()
{

}

HRESULT CDynamicCamera::Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(CCamera::Init_Camera(), E_FAIL);

	return S_OK;
}

Engine::_int CDynamicCamera::Update_Object(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	if (false == m_bFix)
	{
		Mouse_Fix();
		Mouse_Move();
	}

	return CCamera::Update_Component(fTimeDelta);
}

void CDynamicCamera::Key_Input(const _float& fTimeDelta)
{
	_matrix	matCamWorld;
	D3DXMatrixInverse(&matCamWorld, nullptr, &m_matView);

	if (Key_Pressing(VIR_W))
	{
		//��ġ�� ����
		_vec3 vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		// ���⺤�ͷ� ��ȯ * ����
		_vec3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * 20.f * fTimeDelta;

		// �� ���Ϳ� �����ֱ�
		m_vEye += vLength;
		m_vAt += vLength;
	}
	if (Key_Pressing(VIR_S))
	{
		//��ġ�� ����
		_vec3 vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		// ���⺤�ͷ� ��ȯ * ����
		_vec3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * 20.f * fTimeDelta;

		// �� ���Ϳ� �����ֱ�
		m_vEye -= vLength;
		m_vAt -= vLength;
	}
	if (Key_Pressing(VIR_A))
	{
		//��ġ�� ����
		_vec3 vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		// ���⺤�ͷ� ��ȯ * ����
		_vec3 vLength = *D3DXVec3Normalize(&vRight, &vRight) * 20.f * fTimeDelta;

		// �� ���Ϳ� �����ֱ�
		m_vEye -= vLength;
		m_vAt -= vLength;
	}
	if (Key_Pressing(VIR_D))
	{
		//��ġ�� ����
		_vec3 vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		// ���⺤�ͷ� ��ȯ * ����
		_vec3 vLength = *D3DXVec3Normalize(&vRight, &vRight) * 20.f * fTimeDelta;

		// �� ���Ϳ� �����ֱ�
		m_vEye += vLength;
		m_vAt += vLength;
	}
}

void CDynamicCamera::Mouse_Move(void)
{
	_matrix matCamWorld;
	D3DXMatrixInverse(&matCamWorld, nullptr, &m_matView);

	_vec3 vRotation = MousePos(g_hWnd);
	_float ulValue = D3DXVec3Length(&vRotation);
}

void CDynamicCamera::Mouse_Fix(void)
{

}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{

}

void CDynamicCamera::Free(void)
{

}
