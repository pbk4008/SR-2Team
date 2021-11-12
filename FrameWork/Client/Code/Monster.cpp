#include "pch.h"
#include "Monster.h"

CMonster::CMonster()
{

}

CMonster::CMonster(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}

CMonster::~CMonster()
{

}

void CMonster::Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3	m_vInfo;
	m_pTransform->getAxis(VECAXIS::AXIS_POS, m_vInfo);
	_matrix m_matWorld = m_pTransform->getWorldMatrix();
	_vec3	m_vScale = m_pTransform->getScale();

	_matrix matRot;
	matRot = *ComputeLookAtTarget(pTargetPos);

	m_pTransform->setRotate(matRot);

	_vec3 vDir = *pTargetPos - m_vInfo;

	//_vec3 vDis = m_vInfo - *pTargetPos;
	_vec3 vDis = *pTargetPos - m_vInfo;
	_float fDis = D3DXVec3Length(&vDis);

	if (fDis >= 1.0f)
	{
	m_vInfo += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;
	}
	m_pTransform->setPos(m_vInfo);
}

void CMonster::Chase_Target_Ranged(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3	m_vInfo;
	m_pTransform->getAxis(VECAXIS::AXIS_POS, m_vInfo);
	_matrix m_matWorld = m_pTransform->getWorldMatrix();
	_vec3	m_vScale = m_pTransform->getScale();

	_matrix matRot;
	matRot = *ComputeLookAtTarget(pTargetPos);

	m_pTransform->setRotate(matRot);

	_vec3 vDir = *pTargetPos - m_vInfo;

	//_vec3 vDis = m_vInfo - *pTargetPos;
	_vec3 vDis = *pTargetPos - m_vInfo;
	_float fDis = D3DXVec3Length(&vDis);

	if (fDis >= 7.0f)
	{
		m_vInfo += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;
	}
	m_pTransform->setPos(m_vInfo);
}

void CMonster::Chase_Target_Fly(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3	m_vInfo;
	m_pTransform->getAxis(VECAXIS::AXIS_POS, m_vInfo);
	_matrix m_matWorld = m_pTransform->getWorldMatrix();
	_vec3	m_vScale = m_pTransform->getScale();

	_matrix matRot;
	matRot = *ComputeLookAtTarget(pTargetPos);

	m_pTransform->setRotate(matRot);

	_vec3 vDir = *pTargetPos - m_vInfo;

	//_vec3 vDis = m_vInfo - *pTargetPos;
	_vec3 vDis = *pTargetPos - m_vInfo;
	_float fDis = D3DXVec3Length(&vDis);

	if (fDis >= 2.5f)
	{
		m_vInfo += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;
	}
	m_pTransform->setPos(m_vInfo);
}

_matrix* CMonster::ComputeLookAtTarget(const _vec3* pTargetPos)
{
	_vec3 vRight;
	 m_pTransform->getAxis(VECAXIS::AXIS_RIGHT, vRight);
	 _vec3 vUp;
	 m_pTransform->getAxis(VECAXIS::AXIS_UP, vUp);
	 _vec3 vLook;
	 m_pTransform->getAxis(VECAXIS::AXIS_LOOK, vLook);
	 _vec3 vPos;
	 m_pTransform->getAxis(VECAXIS::AXIS_POS, vPos);

	 _vec3 vAngle = m_pTransform->getAngle();

	 _vec3 vDir = *pTargetPos - vPos;
	 _vec3 vAxis = *D3DXVec3Cross(&vAxis, &vRight, &vLook);
	 _float fDot = acosf(D3DXVec3Dot(D3DXVec3Normalize(&vLook, &vLook), D3DXVec3Normalize(&vDir, &vDir)));
	// _float fDot2 = cosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir), D3DXVec3Normalize(&vLook, &vLook)));
	 _matrix mRot;
	 
	 // _float fRad = D3DXToRadian(fDot);
	
	 /*if(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir), D3DXVec3Normalize(&vLook, &vLook)) > 90.f)
		 return D3DXMatrixRotationAxis(&mRot, &vAxis, fDot);
	 else
		 return D3DXMatrixRotationAxis(&mRot, &vAxis, fDot2);*/

	 return D3DXMatrixRotationAxis(&mRot, &vAxis, fDot);

}