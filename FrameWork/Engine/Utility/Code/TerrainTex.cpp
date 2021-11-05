#include "Engine_Include.h"
#include "TerrainTex.h"

CTerrainTex::CTerrainTex():m_pHeightMap(nullptr)
{
}

CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pDevice) : CVIBuffer(pDevice), m_pHeightMap(nullptr)
{
}

CTerrainTex::CTerrainTex(const CTerrainTex& rhs) : CVIBuffer(rhs), m_pHeightMap(rhs.m_pHeightMap)
{
	m_pHeightMap->AddRef();
}

CTerrainTex::~CTerrainTex()
{
}

HRESULT CTerrainTex::Init_BufferTexture(LPDIRECT3DTEXTURE9 pTexture, const _ulong& dwVtxInv)
{
	m_pHeightMap = pTexture;
	m_pHeightMap->AddRef();

	D3DSURFACE_DESC ddsd;

	m_pHeightMap->GetLevelDesc(0, &ddsd);

	m_dwFVF = FVF_TEX;
	m_dwCntX = ddsd.Width;
	m_dwCntZ = ddsd.Height;
	m_dwInterval = dwVtxInv;
	m_dwTriCnt = (m_dwCntX - 1)*(m_dwCntZ - 1)*2;
	m_dwVtxCnt = m_dwCntX * m_dwCntZ;
	m_dwVtxSize = sizeof(VTXTEX);

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::Init_Buffer(), E_FAIL);

	//TODO:높이맵 생성
	
	D3DLOCKED_RECT d3drc;
	m_pHeightMap->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY);

	VTXTEX* pVertex = nullptr;
	_ulong dwIndex = 0;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_ulong i = 0; i < m_dwCntZ; i++)
	{
		for (_ulong j = 0; j < m_dwCntX; j++)
		{
			dwIndex = i * m_dwCntX + j;
			//(LPWORD)d3drc.pBits : 높이 맵의 색상 값에 접근할 수 있는 주소(간단히 높이맵의 첫번째 주소)
			//(d3drc.Pitch / 4) : 높이 맵의 한줄의 갯수
			//i * (d3drc.Pitch / 4) + j : index
			//간단히 보면 2차원 배열을 1차원으로 표시한다는 식으로 이해하면 빠를 듯
			_float fY = ((_float)(*((LPWORD)d3drc.pBits + i * (d3drc.Pitch / 4) + j) & 0x000000ff)) / 10.f;
			pVertex[dwIndex].vPos = _vec3(_float(j * m_dwInterval), fY, _float(i * m_dwInterval));
			pVertex[dwIndex].vUV = _vec2(_float(j) / (m_dwCntX - 1),_float(i)/(m_dwCntZ-1));
		}
	}
	m_pVB->Unlock();

	_ulong dwTriCnt = 0;

	INDEX16* pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	_ulong dwRow = m_dwCntX-1;
	_ulong dwCol = m_dwCntZ - 1;

	for (_ulong i = 0; i < dwRow ; i++)
	{
		for (_ulong j = 0; i < dwCol; j++)
		{
			dwIndex = i * dwCol + j;

			//오른쪽 위 삼각형
			pIndex[dwTriCnt]._0 = _ushort(dwIndex + dwCol);
			pIndex[dwTriCnt]._1 = _ushort(dwIndex + dwCol +1);
			pIndex[dwTriCnt]._2 = _ushort(dwIndex + 1);
			dwTriCnt++;

			//왼쪽 아래 삼각형
			pIndex[dwTriCnt]._0 = _ushort(dwIndex + dwCol);
			pIndex[dwTriCnt]._1 = _ushort(dwIndex + 1);
			pIndex[dwTriCnt]._2 = _ushort(dwIndex);
			dwTriCnt++;
		}
	}
	m_pIB->Unlock();
	return S_OK;
}

void CTerrainTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CTerrainTex::Clone_Component()
{
	return new CTerrainTex(*this);
}

CTerrainTex* CTerrainTex::Create(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 pTexture, const _ulong& dwVtxInv)
{
	CTerrainTex* pInstance = new CTerrainTex(pDevice);
	if (FAILED(pInstance->Init_BufferTexture(pTexture,dwVtxInv)))
		Safe_Release(pInstance);
	return pInstance;
}

void CTerrainTex::Free()
{
	Safe_Release(m_pHeightMap);
	CVIBuffer::Free();
}

HRESULT Engine::CTerrainTex::Init_BufferNoTexture(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxInv /*= 1*/)
{
	m_dwCntX = dwCntX;
	m_dwCntZ = dwCntZ;
	m_dwFVF = FVF_TEX;
	m_dwInterval = dwVtxInv;
	m_dwTriCnt = (m_dwCntX - 1) * (m_dwCntZ - 1) * 2;
	m_dwVtxCnt = m_dwCntX * m_dwCntZ;
	m_dwVtxSize = sizeof(VTXTEX);

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::Init_Buffer(), E_FAIL);

	
	VTXTEX* pVertex = nullptr;
	_ulong dwIndex = 0;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (_ulong i = 0; i < m_dwCntZ; i++)
	{
		for (_ulong j = 0; j < m_dwCntX; j++)
		{
			dwIndex = i * m_dwCntX + j;
			pVertex[dwIndex].vPos = _vec3(_float(j * m_dwInterval), 0, _float(i * m_dwInterval));
			pVertex[dwIndex].vUV = _vec2(_float(j) / (m_dwCntX - 1), _float(i) / (m_dwCntZ - 1));
		}
	}
	m_pVB->Unlock();

	_ulong dwTriCnt = 0;

	INDEX16* pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	_ulong dwRow = m_dwCntX - 1;
	_ulong dwCol = m_dwCntZ - 1;

	for (_ulong i = 0; i < dwRow; i++)
	{
		for (_ulong j = 0; i < dwCol; j++)
		{
			dwIndex = i * dwCol + j;

			//오른쪽 위 삼각형
			pIndex[dwTriCnt]._0 = _ushort(dwIndex + dwCol);
			pIndex[dwTriCnt]._1 = _ushort(dwIndex + dwCol + 1);
			pIndex[dwTriCnt]._2 = _ushort(dwIndex + 1);
			dwTriCnt++;

			//왼쪽 아래 삼각형
			pIndex[dwTriCnt]._0 = _ushort(dwIndex + dwCol);
			pIndex[dwTriCnt]._1 = _ushort(dwIndex + 1);
			pIndex[dwTriCnt]._2 = _ushort(dwIndex);
			dwTriCnt++;
		}
	}
	m_pIB->Unlock();
	return S_OK;
}

Engine::CTerrainTex* Engine::CTerrainTex::Create(LPDIRECT3DDEVICE9 pDevice, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxInv /*= 1*/)
{
	CTerrainTex* pInstance = new CTerrainTex(pDevice);
	if (FAILED(pInstance->Init_BufferNoTexture(dwCntX, dwCntZ, dwVtxInv)))
		Safe_Release(pInstance);
	return pInstance;
}
