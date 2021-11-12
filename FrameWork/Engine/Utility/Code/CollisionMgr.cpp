#include "Engine_Include.h"
#include "CollisionMgr.h"
#include "Collision.h"
IMPLEMENT_SINGLETON(CCollisionMgr)
CCollisionMgr::CCollisionMgr()
{

}
CCollisionMgr::~CCollisionMgr()
{
}
HRESULT CCollisionMgr::Insert_Collision(CCollision* pCollision)
{
	pCollision->AddRef();
	m_vecCollision.emplace_back(pCollision);
	return S_OK;
}


void CCollisionMgr::TerrainCollision(const _float& fX, _float& fY, const _float& fZ, const _vec3* pTerrainVtxPos,
	const _ulong& dwCntX, 
	const _ulong& dwCntZ, 
	const _ulong& dwVtxItv)
{
	_ulong dwIndex = _ulong(fX / dwVtxItv) * dwCntX + _ulong(fZ/ dwVtxItv);

	_float fWidth = (fX - pTerrainVtxPos[dwIndex + dwCntX].x) / dwVtxItv;
	_float fHeight = (pTerrainVtxPos[dwIndex + dwCntX].z - fZ) / dwVtxItv;

	D3DXPLANE Plane;

	if (fWidth > fHeight)
		D3DXPlaneFromPoints(&Plane, &pTerrainVtxPos[dwIndex + dwCntX], &pTerrainVtxPos[dwIndex + dwCntX + 1], &pTerrainVtxPos[dwIndex + 1]);
	else
		D3DXPlaneFromPoints(&Plane, &pTerrainVtxPos[dwIndex + dwCntX], &pTerrainVtxPos[dwIndex + 1], &pTerrainVtxPos[dwIndex]);

	//ax + by + cz + d = 0;
	fY = ((-Plane.a * fX) + (-Plane.c * fZ) + (-Plane.d)) / Plane.b;
}

void CCollisionMgr::Collision(CCollision* pCollision, COLLISIONTAG eTag)
{
	for (auto& pCol : m_vecCollision)
	{
		if (!pCollision->getActive())
			return;
		if (pCol->getTag() != eTag)
			continue;
		if (!pCol->getActive())
			continue;
		if (pCol->getTrigger() != COLLISIONTRIGGER::HIT)
			continue;
		if (CollisionCheck(pCollision, pCol))
		{
			pCol->setHit(true);
			pCollision->setHit(true);
			return;
		}
	}
}

void CCollisionMgr::ClearCollisionList()
{
	for_each(m_vecCollision.begin(), m_vecCollision.end(), DeleteObj);
	m_vecCollision.clear();
}

_bool CCollisionMgr::CollisionCheck(CCollision* pCol, CCollision* pCollider)
{
	_vec3 pColPos = pCol->getCenter();
	_vec3 pColliderPos = pCollider->getCenter();

	_vec3 vDir = pColliderPos - pColPos;
	_float fLen = D3DXVec3Length(&vDir);

	_float fColRadius = pCol->getRadius();
	_float fColliderRadius = pCollider->getRadius();

	_float fRadiusSum = fColRadius + fColliderRadius;

	if (fRadiusSum > fLen)
		return true;
	
	return false;
}




void CCollisionMgr::Free()
{
	ClearCollisionList();
}
