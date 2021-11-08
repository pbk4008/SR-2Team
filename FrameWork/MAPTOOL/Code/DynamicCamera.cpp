#include "pch.h"
#include "DynamicCamera.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{

}

CDynamicCamera::CDynamicCamera(const CDynamicCamera& rhs)
	:CCamera(rhs),m_bFix(rhs.m_bFix)
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
		//Mouse_Fix();
		if (Key_Down(VIR_RBUTTON))
			SetFirstMousePos(g_hWnd);
		if (Key_Pressing(VIR_RBUTTON))
			Mouse_Move();
	}

	return CCamera::Update_Component(fTimeDelta);
}

CComponent* CDynamicCamera::Clone_Component()
{
	return new CDynamicCamera(*this);
}

void CDynamicCamera::Key_Input(const _float& fTimeDelta)
{
	_matrix	matCamWorld;
	D3DXMatrixInverse(&matCamWorld, nullptr, &m_matView);

	if (Key_Pressing(VIR_W))
	{
		//위치값 저장
		_vec3 vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		// 방향벡터로 변환 * 길이
		_vec3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * 20.f * fTimeDelta;

		// 각 벡터에 더해주기
		m_vEye += vLength;
		m_vAt += vLength;
	}
	if (Key_Pressing(VIR_S))
	{
		//위치값 저장
		_vec3 vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		// 방향벡터로 변환 * 길이
		_vec3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * 20.f * fTimeDelta;

		// 각 벡터에 더해주기
		m_vEye -= vLength;
		m_vAt -= vLength;
	}
	if (Key_Pressing(VIR_A))
	{
		//위치값 저장
		_vec3 vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		// 방향벡터로 변환 * 길이
		_vec3 vLength = *D3DXVec3Normalize(&vRight, &vRight) * 20.f * fTimeDelta;

		// 각 벡터에 더해주기
		m_vEye -= vLength;
		m_vAt -= vLength;
	}
	if (Key_Pressing(VIR_D))
	{
		//위치값 저장
		_vec3 vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		// 방향벡터로 변환 * 길이
		_vec3 vLength = *D3DXVec3Normalize(&vRight, &vRight) * 20.f * fTimeDelta;

		// 각 벡터에 더해주기
		m_vEye += vLength;
		m_vAt += vLength;
	}
}

void CDynamicCamera::Mouse_Move(void)
{
	_matrix matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL , &m_matView);

	_vec3 dwMouseMove ;
	
	//===== 위 아래 회전
	dwMouseMove = MousePos(g_hWnd);
	if (dwMouseMove.y )
	{
		_vec3 vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3 vLook = m_vAt - m_vEye;

		_matrix	matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove.y / 10.f ));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}
	// ==== 좌 우 회전

	if (dwMouseMove.x)
	{
		_vec3	vUp = { 0.f,1.f,0.f };
		//memcpy(&vUp, &matCamWorld.m[1][0], sizeof(_vec3) );

		_vec3 vLook = m_vAt - m_vEye;

		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove.x / 10.f  ));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}

}

void CDynamicCamera::Mouse_Fix(void)
{
	POINT	ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CDynamicCamera* pInstance = new CDynamicCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
		Safe_Release(pInstance);

	return pInstance;

}

void CDynamicCamera::Free(void)
{
	CCamera::Free();
}
