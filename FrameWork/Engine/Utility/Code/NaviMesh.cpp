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

void CNaviMesh::Create_Cell(const _vec3* pVtx)
{
	for (_ulong i = 0; i < m_dwCntZ-1; i++)
	{
		for (_ulong j = 0; j < m_dwCntX-1; j++)
		{
			_ulong dwIndex = m_dwCntX * i + j;
			//���� �Ʒ� Cell
			CELL* pCell = new CELL;

			pCell->vPoint[0] = pVtx[dwIndex];
			pCell->vPoint[1] = pVtx[dwIndex + m_dwCntX];
			pCell->vPoint[2] = pVtx[dwIndex + 1];

			pCell->vMid[0] = (pCell->vPoint[0] + pCell->vPoint[1]) * 0.5f;
			pCell->vMid[1] = (pCell->vPoint[1] + pCell->vPoint[2]) * 0.5f;
			pCell->vMid[2] = (pCell->vPoint[2] + pCell->vPoint[0]) * 0.5f;

			pCell->vCenter = (pCell->vPoint[0] + pCell->vPoint[1] + pCell->vPoint[2]) / 3;
			pCell->dwIndex = m_vecNaviCell.size();
			m_vecNaviCell.emplace_back(pCell);

			//������ �� Cell
			pCell = new CELL;
			pCell->vPoint[0] = pVtx[dwIndex + m_dwCntX];
			pCell->vPoint[1] = pVtx[dwIndex + m_dwCntX + 1];
			pCell->vPoint[2] = pVtx[dwIndex + 1];

			pCell->vMid[0] = (pCell->vPoint[0] + pCell->vPoint[1]) * 0.5f;
			pCell->vMid[1] = (pCell->vPoint[1] + pCell->vPoint[2]) * 0.5f;
			pCell->vMid[2] = (pCell->vPoint[2] + pCell->vPoint[0]) * 0.5f;

			pCell->vCenter = (pCell->vPoint[0] + pCell->vPoint[1] + pCell->vPoint[2]) / 3;
			pCell->dwIndex = m_vecNaviCell.size();
			m_vecNaviCell.emplace_back(pCell);
		}
	}
}

void CNaviMesh::Check_Shape(const _vec3& pCenter, RECT* rc)
{
	_vec3 vRayDir = { 0.f,-1.f,0.f };
	_int iIndex = 0;
	for (auto pCell : m_vecNaviCell)
	{
		if (D3DXIntersectTri(&pCell->vPoint[0], &pCell->vPoint[1], &pCell->vPoint[2], &pCenter, &vRayDir, nullptr, nullptr, nullptr))
		{
			pCell->bCheck = false;
			break;
		}
		iIndex++;
		if (iIndex >= m_vecNaviCell.size())
			iIndex = m_vecNaviCell.size() - 1;
	}
	Check_Index(iIndex,rc,m_dwCntX, m_dwCntZ);
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

	for (_ulong i = 0; i < m_dwCntZ; i++)
	{
		for (_ulong j = 0; j < m_dwCntX; j++)
		{
			_ulong iIndex = m_dwCntX * i + j;
			if (!m_vecNaviCell[iIndex]->bCheck)
				continue;
			//������ �� �ﰢ��
			if (iIndex % 2 == 1)
			{
				//�� ����
				if (iIndex < (2 * (m_dwCntX-1) * (m_dwCntZ - 2)))//���� ������ �ƴҶ�
				{
					if (m_vecNaviCell[iIndex + ((m_dwCntX-1) * 2) - 1]->bCheck)
					{
			
						m_vecNaviMesh[iIndex].emplace_back(m_vecNaviCell[iIndex + ((m_dwCntX-1) * 2) - 1]);
					}
				}
				//������ ����
				if (iIndex % ((m_dwCntX-1)*2) != (m_dwCntX-1)*2-1)//���� �������� �ƴ϶��
				{
					if (m_vecNaviCell[iIndex + 1]->bCheck)
					{
						m_vecNaviMesh[iIndex].emplace_back(m_vecNaviCell[iIndex + 1]);
					}
				}
				//���� ����
				if (m_vecNaviCell[iIndex - 1]->bCheck)
				{
					m_vecNaviMesh[iIndex].emplace_back(m_vecNaviCell[iIndex - 1]);
				}
			}
			//���� �Ʒ� �ﰢ��
			else
			{
				//���� ����
				if (iIndex % ((m_dwCntX-1) * 2) != 0)//���� ������ �ƴ϶��
				{
					if (m_vecNaviCell[iIndex - 1]->bCheck)
					{
						m_vecNaviMesh[iIndex].emplace_back(m_vecNaviCell[iIndex - 1]);
					}
				}
				//�Ʒ��� ����
				if (iIndex >= ((m_dwCntX-1) * 2))//���� �Ʒ����� �ƴ϶��
				{
					if (m_vecNaviCell[iIndex - ((m_dwCntX-1) * 2) + 1]->bCheck)
					{
						m_vecNaviMesh[iIndex].emplace_back(m_vecNaviCell[iIndex - ((m_dwCntX - 1) * 2) + 1]);
					}
				}
				//�����ʿ���
				if (m_vecNaviCell[iIndex + 1]->bCheck)
				{
					m_vecNaviMesh[iIndex].emplace_back(m_vecNaviCell[iIndex + 1]);
				}
			}
		}
	}
}

void CNaviMesh::Check_Index(_ulong iIndex, RECT* rc, const _ulong& dwCntX, const _ulong& dwCntZ)
{
	_bool bEnd[3] = {false, false, false};
	for (_int i = 0; i < 3; i++)
	{
		POINT pt = { _long(m_vecNaviCell[iIndex]->vPoint[i].x), _long(m_vecNaviCell[iIndex]->vPoint[i].z) };
		if (PtInRect(rc, pt))
			bEnd[i] = true;
	}
	if (!bEnd[0] && !bEnd[1] && !bEnd[2])
		return;

	if (iIndex % 2 == 0)//�Ʒ� �ﰢ��
	{
		//if(iIndex<)
		//������
		//�Ʒ���
		//����
	}
	else
	{
		//����
		//������
		//�Ʒ���
	}
	//�Ʒ���
	//if (iIndex < m_dwCntX * 2 && iIndex%2 == 0)
	//{
	//	if (m_vecNaviCell[iIndex + 1]->bCheck)
	//	{
	//		m_vecNaviCell[iIndex + 1]->bCheck = false;
	//		Check_Index(iIndex + 1, rc);
	//	}
	//}
	////����
	//else if (iIndex % (m_dwCntX * 2) == 0)
	//{
	//	if (m_vecNaviCell[iIndex - (m_dwCntX * 2) + 1]->bCheck)
	//	{
	//		m_vecNaviCell[iIndex - (m_dwCntX * 2) + 1]->bCheck = false;
	//		Check_Index(iIndex - (m_dwCntX * 2) + 1, rc);
	//	}
	//	if (m_vecNaviCell[iIndex + 1]->bCheck)
	//	{
	//		m_vecNaviCell[iIndex + 1]->bCheck = false;
	//		Check_Index(iIndex + 1, rc);
	//	}
	//}
	////������
	//else if (iIndex % (m_dwCntX * 2) == m_dwCntX - 1)
	//{
	//	if (m_vecNaviCell[iIndex + (m_dwCntX * 2) - 1]->bCheck)
	//	{
	//		m_vecNaviCell[iIndex + (m_dwCntX * 2) - 1]->bCheck = false;
	//		Check_Index(iIndex + (m_dwCntX * 2) - 1, rc);
	//	}
	//	if (m_vecNaviCell[iIndex - 1]->bCheck)
	//	{
	//		m_vecNaviCell[iIndex - 1]->bCheck = false;
	//		Check_Index(iIndex - 1, rc);
	//	}
	//}
	////����
	//else if (iIndex > (2 * m_dwCntX * (m_dwCntZ - 1)) && iIndex % 2 == 1)
	//{
	//	if (m_vecNaviCell[iIndex + 1]->bCheck)
	//	{
	//		m_vecNaviCell[iIndex + 1]->bCheck = false;
	//		Check_Index(iIndex + 1, rc);
	//	}
	//	if (m_vecNaviCell[iIndex - 1]->bCheck)
	//	{
	//		m_vecNaviCell[iIndex - 1]->bCheck = false;
	//		Check_Index(iIndex - 1, rc);
	//	}
	//}
	//else
	//{
	//	//������ �� �ﰢ��
	//	if (iIndex % 2 == 0)
	//	{
	//		if (m_vecNaviCell[iIndex - 1]->bCheck)
	//		{
	//			m_vecNaviCell[iIndex - 1]->bCheck = false;
	//			Check_Index(iIndex - 1, rc);
	//		}
	//		if (m_vecNaviCell[iIndex + 1]->bCheck) 
	//		{
	//			m_vecNaviCell[iIndex + 1]->bCheck = false;
	//			Check_Index(iIndex + 1, rc);
	//		}
	//		if (m_vecNaviCell[iIndex + (m_dwCntX * 2) - 1]->bCheck)
	//		{
	//			m_vecNaviCell[iIndex + (m_dwCntX * 2) - 1]->bCheck = false;
	//			Check_Index(iIndex + (m_dwCntX * 2) - 1, rc);
	//		}
	//	}
	//	//���� �Ʒ� �ﰢ��
	//	else
	//	{
	//		if (m_vecNaviCell[iIndex - 1]->bCheck)
	//		{
	//			m_vecNaviCell[iIndex - 1]->bCheck = false;
	//			Check_Index(iIndex - 1, rc);
	//		}
	//		if (m_vecNaviCell[iIndex + 1]->bCheck)
	//		{
	//			m_vecNaviCell[iIndex + 1]->bCheck = false;
	//			Check_Index(iIndex + 1, rc);
	//		}
	//		if (m_vecNaviCell[iIndex - (m_dwCntX * 2) + 1]->bCheck)
	//		{
	//			m_vecNaviCell[iIndex - (m_dwCntX * 2) + 1]->bCheck = false;
	//			Check_Index(iIndex - (m_dwCntX * 2) + 1, rc);
	//		}
	//	}
	//}
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
	/*for (auto pCell : m_vecNaviCell)
	{
		Safe_Release(pCell->pVtx);
		Safe_Delete(pCell);
	}*/
	for_each(m_vecNaviCell.begin(), m_vecNaviCell.end(), Safe_Delete<CELL*>);
	m_vecNaviCell.clear();
	m_vecNaviCell.shrink_to_fit();

}

void CNaviMesh::setVtxCnt(const _vec3* pVtx, const _ulong& dwCntX, const _ulong& dwCntZ)
{
	m_dwCntX = dwCntX;
	m_dwCntZ = dwCntZ;
	Create_Cell(pVtx);
}
