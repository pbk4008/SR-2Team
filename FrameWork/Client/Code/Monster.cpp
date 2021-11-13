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

	if (fDis >= 15.0f)
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
	_matrix matView, matBill;
	D3DXMatrixIdentity(&matBill);

	_matrix matWorld;
	matWorld = m_pTransform->getWorldMatrix();

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	return &matBill;
}