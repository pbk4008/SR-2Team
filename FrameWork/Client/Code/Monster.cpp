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
	//	0:MAT_RIGHT 1 : MAT_UP 2 : MAT_LOOK 3 : MAT_POS
	// 
	//_vec3 vDir = *pTargetPos - m_vInfo[INFO_POS];

	_vec3 vDir = *pTargetPos - m_vInfo[3]; 

	m_vInfo[3] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	_matrix matScale, matRot, matTrans;

	matRot = *ComputeLookAtTarget(pTargetPos);

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m)vScale.z);

	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS.z]);

	m_matWorld = matScale, matRot, matTrans;
}

void CMonster::ComputeLookAtTarget(const _vec3* pTargetPos)
{
	/*
	ComputeLookAtTarget --> CTransform

	_vec3 vDir = *pTarget - m_vInfo[INFO_POS];
	_vec3 vAxis = *D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir);
	_matrix matRot =;
	_vec3 vUp;
	_float fDot = acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir), D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]))));

	return D3DXMatrixRotationAxis(&matRot, &vAxis, fDot);
	*/
}

