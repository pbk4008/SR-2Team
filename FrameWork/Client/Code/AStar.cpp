#include "pch.h"
#include "AStar.h"
#include "Terrain.h"
#include "NaviMesh.h"

CAstar::CAstar() :m_iStartIndex(0)
{
}

CAstar::~CAstar()
{
}

HRESULT CAstar::Init_AStar(const _vec3& vPos)
{
	vector<CGameObject*>* pObjList = GetGameObjects(LAYERID::ENVIRONMENT, GAMEOBJECTID::TERRAIN);

	CTerrain* pTerrain = nullptr;
	_float fMin = 1000.f;
	for (auto iter : *pObjList)
	{
		_vec3 vTerrainPos = iter->getTransform()->getPos();
		_float fLen = abs(vPos.y - vTerrainPos.y);
		if (fMin > fLen)
		{
			fMin = fLen;
			pTerrain = static_cast<CTerrain*>(iter);
		}
	}
	 m_vecCell = pTerrain->getNaviMesh()->getNaviCell();
	 m_vecMesh = pTerrain->getNaviMesh()->getMesh();
	return S_OK;
}

void CAstar::StartAstar(const _vec3& vStart, const _vec3& vGoal)
{
	m_OpenList.clear();
	m_CloseList.clear();
	m_BestList.clear();

	m_iStartIndex = getIndex(vStart);
	_int dwEndIndex = getIndex(vGoal);
	
	if (m_iStartIndex == -1 || dwEndIndex == -1)
		return;
	if (m_iStartIndex == dwEndIndex)
		return;
	if (!m_vecCell[dwEndIndex]->bCheck)
		return;
	if (PathFind(m_iStartIndex, dwEndIndex,vGoal))
		MakeRoute(dwEndIndex);
}

_int CAstar::getIndex(const _vec3& vPos)
{
	_ulong dwSize = m_vecCell.size();
	for(_ulong i = 0; i<dwSize; i++)
	{
		if(IsPicking(vPos, i))
			return i;
	}
	return -1;
}
_bool CAstar::getObstacle(const _ulong& dwIndex)
{
	return m_vecCell[dwIndex]->bCheck;
}
bool CAstar::PathFind(const _int& dwStartIndex, const _int& dwGoalIndex, const _vec3& vGoal)
{
	if (!m_OpenList.empty())
		m_OpenList.pop_front();
	m_CloseList.emplace_back(dwStartIndex);
	
	//99-194
	
	for (auto& pCell : m_vecMesh[dwStartIndex])
	{
		if (dwGoalIndex == pCell->dwIndex)
		{
			pCell->iParentIndex = dwStartIndex;
			return true;
		}
		if (CheckOpen(pCell->dwIndex) || CheckClose(pCell->dwIndex))
			continue;
		pCell->iParentIndex = dwStartIndex;
		m_OpenList.emplace_back(pCell->dwIndex);
	}

	if (m_OpenList.empty())
		return false;
	m_OpenList.sort([&](_int iPreIndex, _int iNextIndex)
		{
			_vec3 vPreToStart = m_vecCell[iPreIndex]->vCenter - m_vecCell[m_iStartIndex]->vCenter;
			_vec3 vPreToGoal = m_vecCell[iPreIndex]->vCenter -m_vecCell[dwGoalIndex]->vCenter;
			_float fPreDist = D3DXVec3Length(&vPreToStart) + D3DXVec3Length(&vPreToGoal);
			
			_vec3 vNextToStart = m_vecCell[iNextIndex]->vCenter - m_vecCell[m_iStartIndex]->vCenter;
			_vec3 vNextToGoal = m_vecCell[iNextIndex]->vCenter - m_vecCell[dwGoalIndex]->vCenter;
			_float fNextDist = D3DXVec3Length(&vNextToStart) + D3DXVec3Length(&vNextToGoal);

			return fPreDist < fNextDist;
		});

	return PathFind(m_OpenList.front(), dwGoalIndex,vGoal);
}

void CAstar::MakeRoute(const _int& iGoalIndex)
{
	_int iParentIndex = m_vecCell[iGoalIndex]->iParentIndex;
	m_BestList.emplace_back(m_vecCell[iGoalIndex]);

	while (true)
	{
		if (iParentIndex == m_iStartIndex)
			break;
		m_BestList.emplace_front(m_vecCell[iParentIndex]);
		iParentIndex = m_vecCell[iParentIndex]->iParentIndex;
	}
}

bool CAstar::CheckOpen(const _ulong& dwIndex)
{
	auto iter = find(m_OpenList.begin(), m_OpenList.end(), dwIndex);
	if (iter != m_OpenList.end())
		return true;

	return false;
}

bool CAstar::CheckClose(const _ulong& dwIndex)
{
	auto iter = find(m_CloseList.begin(), m_CloseList.end(), dwIndex);
	if (iter != m_CloseList.end())
		return true;

	return false;
}

_bool CAstar::IsPicking(const _vec3& vPos, const _ulong& dwIndex)
{
	_vec3 tmpPos = vPos;
	tmpPos.y = 0.f;

	CELL* pCell = m_vecCell[dwIndex];

	_vec3 tmp1 = pCell->vPoint[0] - tmpPos;
	_vec3 tmp2 = pCell->vPoint[1] - tmpPos;
	_vec3 tmp3 = pCell->vPoint[2] - tmpPos;

	_vec3 Result;
	D3DXVec3Cross(&Result,&tmp1, &tmp2);
	if (Result.y < 0)
		return false;
	D3DXVec3Cross(&Result,&tmp1, &tmp3);
	if (Result.y < 0)
		return false;
	D3DXVec3Cross(&Result,&tmp2, &tmp3);
	if (Result.y < 0)
		return false;

	return true;
}

CAstar* CAstar::Create(const _vec3& vPos)
{
	CAstar* pInstance = new CAstar();
	if (FAILED(pInstance->Init_AStar(vPos)))
		Safe_Release(pInstance);
	return pInstance;
}

void CAstar::Free()
{
	m_OpenList.clear();
	m_CloseList.clear();
	m_BestList.clear();
	m_vecCell.clear();
	for (auto pList : m_vecMesh)
		pList.clear();
	m_vecMesh.clear();
}
