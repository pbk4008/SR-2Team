#include "Engine_Include.h"
#include "CollisionMgr.h"
#include "SphereCollision.h"
#include "BoxCollision.h"
IMPLEMENT_SINGLETON(CCollisionMgr)
CCollisionMgr::CCollisionMgr()
{

}
CCollisionMgr::~CCollisionMgr()
{
}
HRESULT CCollisionMgr::Insert_Collision(CCollision* pCollision)
{
	m_vecCollision.emplace_back(pCollision);
	return S_OK;
}

HRESULT CCollisionMgr::Insert_Wall(CCollision* pCollision)
{
	m_vecWall.emplace_back(pCollision);
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

	_float fMin = 1000000000.f;
	if (!m_vecWall.empty())
	{	
		for (auto pWall : m_vecWall)
		{
			CBoxCollision* pBox = static_cast<CBoxCollision*>(pWall);

			_vec3 WallPos = pBox->getCenter();
			_vec3 WallAxis = pBox->getAxis();

			_float fWallMinX, fWallMaxX;
			_float fWallMinZ, fWallMaxZ;

			fWallMinX = WallPos.x - WallAxis.x+0.25f;
			fWallMaxX = WallPos.x + WallAxis.x-0.25f;

			fWallMinZ = WallPos.z - WallAxis.z+0.25f;
			fWallMaxZ = WallPos.z + WallAxis.z-0.25f;

			_bool bCheck = false;
			if (fWallMinX< fX && fWallMaxX>fX&& fWallMinZ< fZ && fWallMaxZ>fZ)
				bCheck = true;
			else
				bCheck = false;
			if (bCheck)
			{
				_float fWallTop = WallPos.y+WallAxis.y;
				_float fDist = abs(fY - fWallTop);
				if (fMin > fDist)
					fMin = fWallTop;
			}
		}
	}
	_float fBottomY = ((-Plane.a * fX) + (-Plane.c * fZ) + (-Plane.d)) / Plane.b;
	if (fMin > abs(fY - fBottomY))
		fMin = fBottomY;

	fY = fMin;
}

void CCollisionMgr::Collision(CCollision* pCollision, COLLISIONTAG eTag)
{
	for (auto& pCol : m_vecCollision)
	{
		_bool CollisionCheck = false;
		if (!pCollision->getActive())
			return;
		if (pCol->getTag() != eTag)
			continue;
		if (!pCol->getActive())
			continue;
		if (pCol->getTrigger() != COLLISIONTRIGGER::HIT)
			continue;

		if (typeid(*pCollision) != typeid(*pCol))
		{
			if (BoxToSphereCollisionCheck(pCollision, pCol))
				CollisionCheck = true;
		}
		else
		{
			if (typeid(*pCollision) == typeid(CSphereCollision))
			{
				if (SphereCollisionCheck(pCollision, pCol))
					CollisionCheck = true;
			}
			else if (typeid(*pCollision) == typeid(CBoxCollision))
			{
			if(BoxCollisionCheck(pCollision,pCol))
				CollisionCheck = true;
			}
		}
		if (CollisionCheck)
		{
			if (pCollision->getTrigger() == COLLISIONTRIGGER::INTERACT)
			{
				pCollision->setHit(true);
				pCollision->setCollider(pCol);
			}
			pCol->setHit(true);
			pCol->setCollider(pCollision);
			return;
		}
	}
}

void CCollisionMgr::ClearCollision()
{
	for_each(m_vecCollision.begin(), m_vecCollision.end(), DeleteObj);
	m_vecWall.clear();
}

void CCollisionMgr::ClearWall()
{
	for_each(m_vecWall.begin(), m_vecWall.end(), DeleteObj);
	m_vecCollision.clear();
}


void CCollisionMgr::WallCollision(CCollision* pCollsion, _vec3& MoveVec)
{
	for (auto pCollider : m_vecWall)
	{
		BoxToSphereCollisionCheck(pCollsion, pCollider, &MoveVec);
	}
}


_bool CCollisionMgr::SphereCollisionCheck(CCollision* pCol, CCollision* pCollider)
{
	CSphereCollision* pSphereCol = static_cast<CSphereCollision*>(pCol);
	CSphereCollision* pSpherepCollider = static_cast<CSphereCollision*>(pCollider);

	_vec3 pColPos = pSphereCol->getCenter();
	_vec3 pColliderPos = pSpherepCollider->getCenter();

	_vec3 vDir = pColliderPos - pColPos;
	_float fLen = D3DXVec3Length(&vDir);

	_float fColRadius = pSphereCol->getRadius();
	_float fColliderRadius = pSpherepCollider->getRadius();

	_float fRadiusSum = fColRadius + fColliderRadius;

	if (fRadiusSum > fLen)
		return true;
	
	return false;
}

_bool CCollisionMgr::BoxCollisionCheck(CCollision* pCol, CCollision* pCollider)
{
	CBoxCollision* pBoxCol = static_cast<CBoxCollision*>(pCol);
	CBoxCollision* pBoxCollider = static_cast<CBoxCollision*>(pCollider);

	_vec3 pColPos = pBoxCol->getCenter();
	_vec3 pColliderPos = pBoxCollider->getCenter();

	//����
	_vec3 vColAxis = pBoxCol->getAxis();
	_vec3 vColliderAxis = pBoxCollider->getAxis();

	//�ݸ����� AABB
	AABB pColAABB =
	{
		pColPos.x - vColAxis.x,
		pColPos.x + vColAxis.x,
		pColPos.y - vColAxis.y,
		pColPos.y + vColAxis.y,
		pColPos.z - vColAxis.z,
		pColPos.z + vColAxis.z,
	};
	//�ݸ����� AABB
	AABB pColiderAABB =
	{
		pColliderPos.x - vColliderAxis.x,
		pColliderPos.x + vColliderAxis.x,
		pColliderPos.y - vColliderAxis.y,
		pColliderPos.y + vColliderAxis.y,
		pColliderPos.z - vColliderAxis.z,
		pColliderPos.z + vColliderAxis.z,
	};
	
	_bool bCheck = true;

	if (pColAABB.fMinX > pColiderAABB.fMaxX
		|| pColAABB.fMaxX < pColiderAABB.fMinX)
		bCheck = false;
	if (pColAABB.fMinY > pColiderAABB.fMaxY
		|| pColAABB.fMaxY < pColiderAABB.fMinY)
		bCheck = false;
	if (pColAABB.fMinZ > pColiderAABB.fMaxZ
		|| pColAABB.fMaxZ < pColiderAABB.fMinZ)
		bCheck = false;

	return bCheck;
}

_bool CCollisionMgr::BoxToSphereCollisionCheck(CCollision* pCol, CCollision* pCollider, _vec3* pVec)
{
	CBoxCollision* pBox = nullptr;
	CSphereCollision* pSphere = nullptr;
	if (typeid(*pCol) == typeid(CBoxCollision))
	{
		pBox = static_cast<CBoxCollision*>(pCol);
		pSphere = static_cast<CSphereCollision*>(pCollider);
	}
	else if (typeid(*pCol) == typeid(CSphereCollision))
	{
		pBox = static_cast<CBoxCollision*>(pCollider);
		pSphere = static_cast<CSphereCollision*>(pCol);
	}

	_vec3 pBoxPos = pBox->getCenter();
	_vec3 pSpherePos = pSphere->getCenter();

	//����
	_vec3 pBoxAxis = 0.5f*pBox->getAxis();
	_vec3 pSphereAxis = 0.5f*_vec3(pSphere->getRadius(), pSphere->getRadius(), pSphere->getRadius());

	//�ݸ����� AABB
	AABB pBoxAABB =
	{
		pBoxPos.x - pBoxAxis.x,
		pBoxPos.x + pBoxAxis.x,
		pBoxPos.y - pBoxAxis.y,
		pBoxPos.y + pBoxAxis.y,
		pBoxPos.z - pBoxAxis.z,
		pBoxPos.z + pBoxAxis.z,
	};
	//�ݸ����� AABB
	AABB pSphereAABB =
	{
		pSpherePos.x - pSphereAxis.x,
		pSpherePos.x + pSphereAxis.x,
		pSpherePos.y - pSphereAxis.y,
		pSpherePos.y + pSphereAxis.y,
		pSpherePos.z - pSphereAxis.z,
		pSpherePos.z + pSphereAxis.z,
	};

	_bool bCheck = true;

	if (pBoxAABB.fMinX > pSphereAABB.fMaxX
		|| pBoxAABB.fMaxX < pSphereAABB.fMinX)
		bCheck = false;
	if (pBoxAABB.fMinY > pSphereAABB.fMaxY
		|| pBoxAABB.fMaxY < pSphereAABB.fMinY)
		bCheck = false;
	if (pBoxAABB.fMinZ > pSphereAABB.fMaxZ
		|| pBoxAABB.fMaxZ < pSphereAABB.fMinZ)
		bCheck = false;

	if (bCheck)
	{
		if(!pVec)
			return bCheck;
		if (pBoxAABB.fMinX < pSphereAABB.fMinX &&
			pBoxAABB.fMaxX < pSphereAABB.fMaxX)
			pVec->x = pBoxAABB.fMaxX - pSphereAABB.fMinX;
		else
			pVec->x = pSphereAABB.fMaxX - pBoxAABB.fMinX;

		if (pBoxAABB.fMinZ < pSphereAABB.fMinZ &&
			pBoxAABB.fMaxZ < pSphereAABB.fMaxZ)
			pVec->z = pBoxAABB.fMaxZ - pSphereAABB.fMinZ;
		else
			pVec->z = pSphereAABB.fMaxZ - pBoxAABB.fMinZ;

		if (pVec->y!=0)
		{
			*pVec = _vec3(0.f, 0.f, 0.f);
			return bCheck;
		}
		if (pVec->x < pVec->z)
		{
			pVec->z = 0.f;
			if (pBoxPos.x > pSpherePos.x)
				*pVec *= -1;
		}
		else
		{
			pVec->x = 0.f;
			if (pBoxPos.z > pSpherePos.z)
				*pVec *= -1;
		}
	}
	return bCheck;
}

void CCollisionMgr::Free()
{
	for_each(m_vecCollision.begin(), m_vecCollision.end(), DeleteObj);
	m_vecWall.clear();
	for_each(m_vecWall.begin(), m_vecWall.end(), DeleteObj);
	m_vecCollision.clear();
}
