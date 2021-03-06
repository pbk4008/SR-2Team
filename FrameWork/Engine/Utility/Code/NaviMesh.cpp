#include "Engine_Include.h"
#include "NaviMesh.h"

CNaviMesh::CNaviMesh() : m_dwCntX(0), m_dwCntZ(0)
{
}

CNaviMesh::CNaviMesh(LPDIRECT3DDEVICE9 pDevice) : CComponent(pDevice),  m_dwCntX(0), m_dwCntZ(0)
{
}

CNaviMesh::CNaviMesh(const CNaviMesh& rhs) : CComponent(rhs), m_dwCntX(rhs.m_dwCntX), m_dwCntZ(rhs.m_dwCntX)
{
}

CNaviMesh::~CNaviMesh()
{
}

HRESULT CNaviMesh::Init_Navimesh(const _ulong& dwCntX, const _ulong& dwCntZ)
{
	m_dwCntX = dwCntX;
	m_dwCntZ = dwCntZ;

	return S_OK;
}

_int CNaviMesh::Update_Component(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CComponent::Update_Component(fDeltaTime);

	return iExit;
}

void CNaviMesh::Render_NaviMesh()
{
}

CComponent* CNaviMesh::Clone_Component()
{
	return new CNaviMesh(*this);
}

void CNaviMesh::Create_Cell(const _vec3* pVtx, const _float& fY)
{
	for (_ulong i = 0; i < m_dwCntZ-1; i++)
	{
		for (_ulong j = 0; j < m_dwCntX-1; j++)
		{
			_ulong dwIndex = m_dwCntX * i + j;
			//왼쪽 아래 Cell
			CELL* pCell = new CELL;

			pCell->vPoint[0] = pVtx[dwIndex];
			pCell->vPoint[1] = pVtx[dwIndex + m_dwCntX];
			pCell->vPoint[2] = pVtx[dwIndex + 1];

			for (_int i = 0; i < 3; i++)
				pCell->vPoint[i].y = fY;

			pCell->vMid[0] = (pCell->vPoint[0] + pCell->vPoint[1]) * 0.5f;
			pCell->vMid[1] = (pCell->vPoint[1] + pCell->vPoint[2]) * 0.5f;
			pCell->vMid[2] = (pCell->vPoint[2] + pCell->vPoint[0]) * 0.5f;

			pCell->vCenter = (pCell->vPoint[0] + pCell->vPoint[1] + pCell->vPoint[2]) / 3;
			pCell->dwIndex = m_vecNaviCell.size();
			m_vecNaviCell.emplace_back(pCell);

			//오른쪽 위 Cell
			pCell = new CELL;
			pCell->vPoint[0] = pVtx[dwIndex + m_dwCntX];
			pCell->vPoint[1] = pVtx[dwIndex + m_dwCntX + 1];
			pCell->vPoint[2] = pVtx[dwIndex + 1];

			for (_int i = 0; i < 3; i++)
				pCell->vPoint[i].y = fY;

			pCell->vMid[0] = (pCell->vPoint[0] + pCell->vPoint[1]) * 0.5f;
			pCell->vMid[1] = (pCell->vPoint[1] + pCell->vPoint[2]) * 0.5f;
			pCell->vMid[2] = (pCell->vPoint[2] + pCell->vPoint[0]) * 0.5f;

			pCell->vCenter = (pCell->vPoint[0] + pCell->vPoint[1] + pCell->vPoint[2]) / 3;
			pCell->dwIndex = m_vecNaviCell.size();
			m_vecNaviCell.emplace_back(pCell);
		}
	}
}

void CNaviMesh::Check_Shape(const _vec3& vMinPos, const _vec3& vMaxPos)
{
	for (auto pCell : m_vecNaviCell)
	{
		_vec3 vPos = pCell->vCenter;
		if (vMinPos.x < vPos.x && vMaxPos.x > vPos.x
			&& vMinPos.z< vPos.z && vMaxPos.z>vPos.z)
		{
			if(pCell->bCheck)
				pCell->bCheck = false;
		}
	}
}

void CNaviMesh::Connet_NaviMesh()
{
	if (!m_vecNaviMesh.empty())
	{
		for (auto& pList : m_vecNaviMesh)
			pList.clear();
		m_vecNaviMesh.clear();
	}
	m_vecNaviMesh.resize(m_vecNaviCell.size());
	_int iSize = m_vecNaviMesh.size();
	for (_int iIndex = 0; iIndex < iSize; iIndex++)
	{
		if (!m_vecNaviCell[iIndex]->bCheck)
			continue;
		//오른쪽 위 삼각형
		if (iIndex % 2 == 1)
		{
			//위 연결
			if (iIndex < (2 * (m_dwCntX - 1) * (m_dwCntZ - 2)))//제일 위쪽이 아닐때
			{
				if (m_vecNaviCell[iIndex + ((m_dwCntX - 1) * 2) - 1]->bCheck)
				{

					m_vecNaviMesh[iIndex].emplace_back(m_vecNaviCell[iIndex + ((m_dwCntX - 1) * 2) - 1]);
				}
			}
			//오른쪽 연결
			if (iIndex % ((m_dwCntX - 1) * 2) != (m_dwCntX - 1) * 2 - 1)//제일 오른쪽이 아니라면
			{
				if (m_vecNaviCell[iIndex + 1]->bCheck)
				{
					m_vecNaviMesh[iIndex].emplace_back(m_vecNaviCell[iIndex + 1]);
				}
			}
			//왼쪽 연결
			if (m_vecNaviCell[iIndex - 1]->bCheck)
			{
				m_vecNaviMesh[iIndex].emplace_back(m_vecNaviCell[iIndex - 1]);
			}
		}
		//왼쪽 아래 삼각형
		else
		{
			//왼쪽 연결
			if (iIndex % ((m_dwCntX - 1) * 2) != 0)//제일 왼쪽이 아니라면
			{
				if (m_vecNaviCell[iIndex - 1]->bCheck)
				{
					m_vecNaviMesh[iIndex].emplace_back(m_vecNaviCell[iIndex - 1]);
				}
			}
			//아래쪽 연결
			if (iIndex >= ((m_dwCntX - 1) * 2))//제일 아래쪽이 아니라면
			{
				if (m_vecNaviCell[iIndex - ((m_dwCntX - 1) * 2) + 1]->bCheck)
				{
					m_vecNaviMesh[iIndex].emplace_back(m_vecNaviCell[iIndex - ((m_dwCntX - 1) * 2) + 1]);
				}
			}
			//오른쪽연결
			if (m_vecNaviCell[iIndex + 1]->bCheck)
			{
				m_vecNaviMesh[iIndex].emplace_back(m_vecNaviCell[iIndex + 1]);
			}
		}
	}
}

CNaviMesh* CNaviMesh::Create(LPDIRECT3DDEVICE9 pDevice, const _ulong& dwCntX, const _ulong& dwCntZ)
{
	CNaviMesh* pInstacne = new CNaviMesh(pDevice);
	if (FAILED(pInstacne->Init_Navimesh(dwCntX, dwCntZ)))
		Safe_Release(pInstacne);
	return pInstacne;
}

void CNaviMesh::Free()
{
	CComponent::Free();

	for (auto pMeshList : m_vecNaviMesh)
		pMeshList.clear();
	m_vecNaviMesh.clear();
	m_vecNaviCell.shrink_to_fit();

	for_each(m_vecNaviCell.begin(), m_vecNaviCell.end(), Safe_Delete<CELL*>);
	m_vecNaviCell.clear();
	m_vecNaviCell.shrink_to_fit();

}

void CNaviMesh::setVtxCnt(const _vec3* pVtx, const _ulong& dwCntX, const _ulong& dwCntZ, const _float& fY)
{
	m_dwCntX = dwCntX;
	m_dwCntZ = dwCntZ;
	Create_Cell(pVtx,fY);
}
