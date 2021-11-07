#include "pch.h"
#include "Monster.h"

CMonster::CMonster()
{

}

CMonster::CMonster(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
	ZeroMemory(&pTarget, sizeof(_vec3*));

}

CMonster::~CMonster()
{

}
void CMonster::Set_Target(const _vec3* pTargetPos)
{
	pTargetPos = pTarget;
}

void CMonster::Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3	m_vInfo = m_pTransform->getAxis(VECAXIS::AXIS_POS);
	_matrix m_matWorld = m_pTransform->getWorldMatrix();
	_vec3	m_vScale = m_pTransform->getScale();

	_matrix matRot;
	matRot = *ComputeLookAtTarget(pTargetPos);

	m_pTransform->setRotate(matRot); 

	_vec3 vDir = *pTargetPos - m_vInfo;

	m_vInfo += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	m_pTransform->setPos(m_vInfo);
}

_matrix* CMonster::ComputeLookAtTarget(const _vec3* pTargetPos)
{

	_vec3	m_vInfo = m_pTransform->getAxis(VECAXIS::AXIS_POS);
	_matrix m_matWorld = m_pTransform->getWorldMatrix();
	_vec3	m_vScale = m_pTransform->getScale();

	_vec3 vDir = *pTargetPos - m_vInfo;

	_vec3 vAxis = *D3DXVec3Cross(&vAxis, &m_vInfo, &vDir);

	_matrix matRot;
	_vec3 vUp;
	_float fDot = acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir), D3DXVec3Normalize(&vUp, &m_vInfo)));

	return D3DXMatrixRotationAxis(&matRot, &vAxis, fDot);

}

