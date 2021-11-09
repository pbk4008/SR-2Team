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


void CCollisionMgr::TerrainCollision(_vec3* pPos, const _vec3* pTerrainVtxPos, 
	const _ulong& dwCntX, 
	const _ulong& dwCntZ, 
	const _ulong& dwVtxItv)
{
	_ulong dwIndex = _ulong(pPos->x / dwVtxItv) * dwCntX + _ulong(pPos->z / dwVtxItv);

	_float fWidth = (pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x) / dwVtxItv;
	_float fHeight = (pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z) / dwVtxItv;

	D3DXPLANE Plane;

	if (fWidth > fHeight)
		D3DXPlaneFromPoints(&Plane, &pTerrainVtxPos[dwIndex + dwCntX], &pTerrainVtxPos[dwIndex + dwCntX + 1], &pTerrainVtxPos[dwIndex + 1]);
	else
		D3DXPlaneFromPoints(&Plane, &pTerrainVtxPos[dwIndex + dwCntX], &pTerrainVtxPos[dwIndex + 1], &pTerrainVtxPos[dwIndex]);

	//ax + by + cz + d = 0;
	pPos->y = ((-Plane.a * pPos->x) + (-Plane.c * pPos->z) + (-Plane.d)) / Plane.b;
}

void CCollisionMgr::Collision(CCollision* pCollision, COLLISIONTAG eTag)
{
	for (auto& pCol : m_vecCollision)
	{
		if (pCol->getTag() != eTag)
			continue;
		if (!pCol->getActive())
			continue;
		if (CollisionCheck(pCollision, pCol))
		{
			pCol->setHit(true);
		}
	}
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
	for_each(m_vecCollision.begin(), m_vecCollision.end(), DeleteObj);
	m_vecCollision.clear();
}
