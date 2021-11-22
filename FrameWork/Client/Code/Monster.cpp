#include "pch.h"
#include "Monster.h"
#include "AStar.h"
CMonster::CMonster() :m_pAstar(nullptr), m_bItemCheck(false), m_bFirst(false), m_fNodeLen(0.f), m_bReborn(false)
{
	ZeroMemory(&m_pTargetPos, sizeof(_vec3));
	ZeroMemory(&m_vStart, sizeof(_vec3));
	ZeroMemory(&m_vEnd, sizeof(_vec3));
	ZeroMemory(&m_vDir, sizeof(_vec3));
	ZeroMemory(&m_vNodeFirst, sizeof(_vec3));
	ZeroMemory(&m_vNodeSecond, sizeof(_vec3));
}

CMonster::CMonster(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice), m_pAstar(nullptr), m_bItemCheck(false), m_bFirst(false), m_fNodeLen(0.f), m_bReborn(false)
{
	ZeroMemory(&m_pTargetPos, sizeof(_vec3));
	ZeroMemory(&m_vStart, sizeof(_vec3));
	ZeroMemory(&m_vEnd, sizeof(_vec3));
	ZeroMemory(&m_vDir, sizeof(_vec3));
	ZeroMemory(&m_vNodeFirst, sizeof(_vec3));
	ZeroMemory(&m_vNodeSecond, sizeof(_vec3));
}

CMonster::CMonster(const CMonster& rhs) : CGameObject(rhs), m_pAstar(nullptr), m_pTargetPos(rhs.m_pTargetPos), m_bItemCheck(rhs.m_bItemCheck)
,m_vStart(rhs.m_vStart), m_vEnd(rhs.m_vEnd), m_bFirst(rhs.m_bFirst), m_vDir(rhs.m_vDir),m_vNodeFirst(rhs.m_vNodeFirst)
,m_vNodeSecond(rhs.m_vNodeSecond), m_fNodeLen(rhs.m_fNodeLen), m_bReborn(rhs.m_bReborn)
{
}

CMonster::~CMonster()
{

}

void CMonster::Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3	m_vInfo;
	m_vInfo = m_pTransform->getPos();

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
	m_vInfo = m_pTransform->getPos();

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
	m_vInfo = m_pTransform->getPos();
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
	list<CELL*>& pRoute = m_pAstar->getRoute();
	
	if (pRoute.empty())
		m_pAstar->StartAstar(vStart, vEnd);
	else
	{
		if (!m_bFirst)
		{
			m_bFirst = true;
			m_vStart = pRoute.front()->vCenter;
			m_vEnd = pRoute.back()->vCenter;
			m_vDir = m_vEnd - m_vStart;
			D3DXVec3Normalize(&m_vDir, &m_vDir);
		}
		if (m_fNodeLen < 0.1f)
		{
			m_vNodeFirst = pRoute.front()->vCenter;
			if (m_vNodeFirst == pRoute.back()->vCenter)
			{
				m_bFirst = false;
				pRoute.clear();
				return;
			}
			pRoute.pop_front();
			m_vNodeSecond = pRoute.front()->vCenter;
		}
		_vec3 vDir = m_vNodeSecond - m_vNodeFirst;
		m_fNodeLen = D3DXVec3Length(&vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		_float fDist = m_vDir.z / m_vDir.x;
		_float fTmpDist = vDir.z / vDir.x;
		
		if (abs(fDist - fTmpDist) > 0.3f)
		{
			_vec3 vPos = m_pTransform->getPos();
			_vec3 tmpPos = vPos + m_vDir;
			if (m_pAstar->getObstacle(m_pAstar->getIndex(tmpPos)))
				vPos += m_vDir * fSpeed * fDeltaTime;
			else
				vPos += vDir * fSpeed * fDeltaTime;
			vStart = vPos;
		}
		else
		{
			_vec3 vPos = m_pTransform->getPos();
			 vPos+=m_vDir* fSpeed* fDeltaTime;
			 vStart = vPos;
		}

		m_vNodeFirst += vDir * fSpeed * fDeltaTime;
	}
}

void CMonster::ResetObject()
{
	m_bReborn = true;
	m_bItemCheck = false;
}

void CMonster::Free()
{
	Safe_Release(m_pAstar);
	CGameObject::Free();
}

void CMonster::LoadTransform(const _vec3& vScale, const _vec3& vRotate, const _vec3& vPos)
{
	m_pTransform->setScale(vScale);
	m_pTransform->setAngle(vRotate);
	m_pTransform->setPos(vPos);
	if(!m_bReborn)
		m_pAstar = CAstar::Create(vPos);
}
