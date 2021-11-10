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

	if (fDis >= 1.0)
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

	if (fDis >= 5.0)
	{
		m_vInfo += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;
	}
	m_pTransform->setPos(m_vInfo);
}

_matrix* CMonster::ComputeLookAtTarget(const _vec3* pTargetPos)
{
	_vec3	m_vInfo = m_pTransform->getPos();

	_vec3 vDir = *pTargetPos - m_vInfo;
	_matrix matRot;

	_vec3 vUp;
	_vec3 vUp2;
	m_pTransform->getAxis(VECAXIS::AXIS_UP, vUp2);

	_float fDot = acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir), D3DXVec3Normalize(&vUp, &vUp2)));

	return D3DXMatrixRotationY(&matRot, fDot);
}