#include "pch.h"
#include "Monster.h"
#include "AStar.h"
CMonster::CMonster() :m_pAstar(nullptr), m_bItemCheck(false)
{
	ZeroMemory(&m_pTargetPos, sizeof(_vec3));
}

CMonster::CMonster(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice), m_pAstar(nullptr), m_bItemCheck(false)
{
	ZeroMemory(&m_pTargetPos, sizeof(_vec3));
}

CMonster::CMonster(const CMonster& rhs) : CGameObject(rhs), m_pAstar(nullptr), m_pTargetPos(rhs.m_pTargetPos), m_bItemCheck(rhs.m_bItemCheck)
{
	m_pAstar = CAstar::Create();
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
		MoveRoute(m_vInfo, *pTargetPos, fTimeDelta, fSpeed);
	}
	m_pTransform->setPos(m_vInfo);
}

void CMonster::Chase_Target_Ranged(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3	m_vInfo;
	m_vInfo=m_pTransform->getPos();

	_matrix matRot;
	matRot = *ComputeLookAtTarget(pTargetPos);

	m_pTransform->setRotate(matRot);

	_vec3 vDir = *pTargetPos - m_vInfo;

	//_vec3 vDis = m_vInfo - *pTargetPos;
	_vec3 vDis = *pTargetPos - m_vInfo;
	_float fDis = D3DXVec3Length(&vDis);

	if (fDis >= 5.0f)
	{
		MoveRoute(m_vInfo, *pTargetPos, fTimeDelta, fSpeed);
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
		MoveRoute(m_vInfo, *pTargetPos, fTimeDelta, fSpeed);
	}
	m_pTransform->setPos(m_vInfo);
}

_matrix* CMonster::ComputeLookAtTarget(const _vec3* pTargetPos)
{
	_matrix matView, matBill;
	D3DXMatrixIdentity(&matBill);

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	return &matBill;
}

void CMonster::MoveRoute(_vec3& vStart, const _vec3& vEnd, const _float& fDeltaTime, const _float& fSpeed)
{
	if (m_pTargetPos != vEnd)
	{
		m_pAstar->StartAstar(vStart, vEnd);
		m_pTargetPos = vEnd;
	}
	list<CELL*>& pRoute = m_pAstar->getRoute();
	if (pRoute.empty())
		return;
	_vec3 vMove = pRoute.front()->vCenter;
	vMove.y = 1.f;
	_vec3 vDir = vMove - vStart;

	_float fDist = D3DXVec3Length(&vDir);

	if (fDist < 0.1f)
		pRoute.pop_front();

	D3DXVec3Normalize(&vDir,&vDir);
	_vec3 vMovePos = vDir * fSpeed * fDeltaTime;


	_vec3 vPos = m_pTransform->getPos();
	vPos += vMovePos;
	vStart = vPos;
}

void CMonster::ResetObject()
{
	m_bItemCheck = false;
}

void CMonster::Free()
{
	Safe_Release(m_pAstar);
	CGameObject::Free();
}
