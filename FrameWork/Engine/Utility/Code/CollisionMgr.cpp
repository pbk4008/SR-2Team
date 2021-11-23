#include "Engine_Include.h"
#include "CollisionMgr.h"
#include "Collision.h"
#include "SphereCollision.h"
#include "BoxCollision.h"
#pragma comment(lib,"System.lib")
#include "InputDev.h"

IMPLEMENT_SINGLETON(CCollisionMgr)
CCollisionMgr::CCollisionMgr()
{
	mpInputDev = CInputDev::GetInstance();
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

HRESULT CCollisionMgr::Insert_Wall(CCollision* pCollision)
{
	pCollision->AddRef();
	m_vecWall.emplace_back(pCollision);
	return S_OK;
}


void CCollisionMgr::TerrainCollision(const _float& fX, _float& fY, const _float& fZ, const _vec3* pTerrainVtxPos,
	const _ulong& dwCntX,
	const _ulong& dwCntZ,
	const _ulong& dwVtxItv
	, const _float& fTerrinsYPos)
{
	_ulong dwIndex = _ulong(fX / dwVtxItv) * dwCntX + _ulong(fZ / dwVtxItv);

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
			_vec3 WallAxis = pBox->getScale();

			_float fWallMinX, fWallMaxX;
			_float fWallMinZ, fWallMaxZ;
			_float fWallMinY, fWallMaxY;

			fWallMinX = WallPos.x - (WallAxis.x * 0.5f);
			fWallMaxX = WallPos.x + (WallAxis.x * 0.5f);

			fWallMinZ = WallPos.z - (WallAxis.z * 0.5f);
			fWallMaxZ = WallPos.z + (WallAxis.z * 0.5f);

			fWallMinY = WallPos.y - (WallAxis.y * 0.5f);
			fWallMaxY = WallPos.y + (WallAxis.y * 0.5f);

			_bool bCheck = false;
			if (fWallMinX< fX && fWallMaxX>fX
				&& fWallMinZ< fZ && fWallMaxZ>fZ)
				bCheck = true;
			else
				bCheck = false;

			if (fY < fWallMaxY)
				continue;
			if (bCheck)
			{
				_float fWallTop = WallPos.y + WallAxis.y * 0.5f;
				_float fDist = abs(fY - fWallTop);
				if (fMin > fDist)
					fMin = fWallTop;
			}
		}
	}
	if (dwIndex > (dwCntX - 1) * (dwCntZ - 1))
	{
		fY = fMin;
		return;
	}
	_float fBottomY = ((-Plane.a * fX) + (-Plane.c * fZ) + (-Plane.d)) / Plane.b;
	fBottomY += fTerrinsYPos;
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
				if (BoxCollisionCheck(pCollision, pCol))
					CollisionCheck = true;
			}
		}
		if (CollisionCheck)
		{
			if (pCollision->getTrigger() == COLLISIONTRIGGER::INTERACT)
			{
				pCollision->setHit(true);
			pCol->setHit(true);
			pCol->setCollider(pCollision);
			pCollision->setCollider(pCol);
			return;
		}
	}
}

void CCollisionMgr::ClearCollision()
{
	for_each(m_vecCollision.begin(), m_vecCollision.end(), DeleteObj);
	m_vecCollision.clear();
}

void CCollisionMgr::ClearWall()
{
	for_each(m_vecWall.begin(), m_vecWall.end(), DeleteObj);
	m_vecWall.clear();
}


void CCollisionMgr::WallCollision(CCollision* pCollsion, _vec3& MoveVec,_vec3* walkpower)
{
	for (auto pCollider : m_vecWall)
	{
		if (BoxtoBoxCollisionCheckAABBtoOBB(pCollsion, pCollider, &MoveVec,walkpower))
		{
			break;
		}
	
		/*if (BoxToSphereCollisionCheck(pCollsion, pCollider, &MoveVec))
			break;*/
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
	_vec3 vColAxis = pBoxCol->getScale();
	_vec3 vColliderAxis = pBoxCollider->getScale();

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
	_vec3 pBoxAxis = 0.5f * pBox->getScale();
	_vec3 pSphereAxis = 0.5f * _vec3(pSphere->getRadius(), pSphere->getRadius(), pSphere->getRadius());

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
		if (!pVec)
			return bCheck;
		if (pBoxAABB.fMinX < pSphereAABB.fMinX &&
			pBoxAABB.fMaxX < pSphereAABB.fMaxX)
			pVec->x = pBoxAABB.fMaxX - pSphereAABB.fMinX;
		else
			pVec->x = pSphereAABB.fMaxX - pBoxAABB.fMinX;

		if (pBoxAABB.fMinY < pSphereAABB.fMinY &&
			pBoxAABB.fMaxY < pSphereAABB.fMaxY)
			pVec->y = pBoxAABB.fMaxY - pSphereAABB.fMinY;
		else
			pVec->y = pSphereAABB.fMaxY - pBoxAABB.fMinY;


		if (pBoxAABB.fMinZ < pSphereAABB.fMinZ &&
			pBoxAABB.fMaxZ < pSphereAABB.fMaxZ)
			pVec->z = pBoxAABB.fMaxZ - pSphereAABB.fMinZ;
		else
			pVec->z = pSphereAABB.fMaxZ - pBoxAABB.fMinZ;


		if (pVec->y == 0.f)
		{
			if (min(pVec->x, pVec->z) == pVec->x)
			{
				pVec->z = 0.f;
				if (pBoxPos.x > pSpherePos.x)
					*pVec *= -1;
			}
			else if (min(pVec->x, pVec->z) == pVec->z)
			{
				pVec->x = 0.f;
				if (pBoxPos.z > pSpherePos.z)
					*pVec *= -1;
			}
			return bCheck;
		}

		if (min(min(pVec->x, pVec->z), pVec->y) == pVec->x)
		{
			pVec->y = 0.f;
			pVec->z = 0.f;
			if (pBoxPos.x > pSpherePos.x)
				*pVec *= -1;
		}
		else if (min(min(pVec->x, pVec->z), pVec->y) == pVec->y)
		{
			pVec->x = 0.f;
			pVec->z = 0.f;
			if (pBoxPos.y > pSpherePos.y)
				*pVec *= -1;
		}
		else if (min(min(pVec->x, pVec->z), pVec->y) == pVec->z)
		{
			pVec->x = 0.f;
			pVec->y = 0.f;
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

Engine::_bool Engine::CCollisionMgr::ShpereBoxCollisionCheck(CCollision* pCol, CCollision* pCollider, _vec3* pVec)
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

	//�ڽ� 
	_vec3 pBoxAxis = 0.5f * pBox->getScale();
	_float fSphereRadius = pSphere->getRadius();

	// �����ּ��ִ�
	_vec3 pBoxMin = { pBoxPos.x - pBoxAxis.x,pBoxPos.y - pBoxAxis.y , pBoxPos.z - pBoxAxis.z };
	_vec3 pBoxMax = { pBoxPos.x + pBoxAxis.x, pBoxPos.y + pBoxAxis.y , pBoxPos.z + pBoxAxis.z };

	// �� ���� ���� ������� ã��

	_float x = max(pBoxMin.x, min(pSpherePos.x, pBoxMax.x));
	_float y = max(pBoxMin.y, min(pSpherePos.y, pBoxMax.y));
	_float z = max(pBoxMin.z, min(pSpherePos.z, pBoxMax.z));

	_bool bCheck = true;

	_float distance = sqrtf((x - pSpherePos.x) * (x - pSpherePos.x) + (y - pSpherePos.y) * (y - pSpherePos.y) + (z - pSpherePos.z) * (z - pSpherePos.z));

	if (distance <= fSphereRadius)
	{
		if (!pVec)
			return true;

		if (pBoxMin.x < pSpherePos.x - fSphereRadius &&
			pBoxMax.x < pSpherePos.x + fSphereRadius)
			pVec->x = pBoxMax.x - pSpherePos.x - fSphereRadius;
		else
			pVec->x = pSpherePos.x + fSphereRadius - pBoxMin.x;

		if (pBoxMin.z < pSpherePos.z - fSphereRadius &&
			pBoxMax.z < pSpherePos.z + fSphereRadius)
			pVec->z = pBoxMax.z - pSpherePos.z - fSphereRadius;
		else
			pVec->z = pSpherePos.z + fSphereRadius - pBoxMax.z;

		if (pVec->y != 0)
		{
			*pVec = _vec3(0.f, 0.f, 0.f);
			return bCheck;
		}
		if (pVec->x < pVec->z)
		{
			pVec->z = 0.f;
			if (pBoxPos.x > pSpherePos.x)
			{
				*pVec *= -1;
				D3DXVec3Normalize(pVec, pVec);
			}
		}
		else
		{
			pVec->x = 0.f;
			if (pBoxPos.z > pSpherePos.z)
			{
				*pVec *= -1;
				D3DXVec3Normalize(pVec, pVec);

			}
		}
		return true;
	}
	return false;
}






Engine::CCollisionMgr::INTERVAL Engine::CCollisionMgr::GetInterval(CSphereCollision* pSphere, const _vec3& axis)
{
	_vec3 i = GetMin(pSphere);
	_vec3 a = GetMax(pSphere);

	_vec3 vertex[8] = {
		_vec3(i.x, a.y, a.z),
		_vec3(i.x, a.y, i.z),
		_vec3(i.x, i.y, a.z),
		_vec3(i.x, i.y, i.z),
		_vec3(a.x, a.y, a.z),
		_vec3(a.x, a.y, i.z),
		_vec3(a.x, i.y, a.z),
		_vec3(a.x, i.y, i.z)
	};

	INTERVAL result;
	result.fmin = result.fmax = D3DXVec3Dot(&axis, &vertex[0]);

	for (int i = 1; i < 8; ++i)
	{
		_float projection = D3DXVec3Dot(&axis, &vertex[i]);
		result.fmin = (projection < result.fmin) ?
			projection : result.fmin;
		result.fmax = (projection > result.fmax) ?
			projection : result.fmax;

	}

	return result;

}

Engine::CCollisionMgr::INTERVAL Engine::CCollisionMgr::GetInterval(CBoxCollision* pBox, const _vec3& axis)
{
	_vec3 pBoxPos = pBox->getCenter();
	_vec3 pBoxScale = 0.5f * pBox->getScale();
	_vec3 pBoxRotate = pBox->getAngle();
	_matrix pBoxRot;
	pBoxRot = setAngletomatrix(pBoxRotate);
	_vec3 A[] = {
		_vec3(pBoxRot.m[0][0],pBoxRot.m[0][1],pBoxRot.m[0][2]),
		_vec3(pBoxRot.m[1][0],pBoxRot.m[1][1],pBoxRot.m[1][2]),
		_vec3(pBoxRot.m[2][0],pBoxRot.m[2][1],pBoxRot.m[2][2])
	};
	_vec3 vertex[8];

	vertex[0] = pBoxPos + A[0] * pBoxScale[0] + A[1] * pBoxScale[1] + A[2] * pBoxScale[2];
	vertex[1] = pBoxPos - A[0] * pBoxScale[0] + A[1] * pBoxScale[1] + A[2] * pBoxScale[2];
	vertex[2] = pBoxPos + A[0] * pBoxScale[0] - A[1] * pBoxScale[1] + A[2] * pBoxScale[2];
	vertex[3] = pBoxPos + A[0] * pBoxScale[0] + A[1] * pBoxScale[1] - A[2] * pBoxScale[2];
	vertex[4] = pBoxPos - A[0] * pBoxScale[0] - A[1] * pBoxScale[1] - A[2] * pBoxScale[2];
	vertex[5] = pBoxPos + A[0] * pBoxScale[0] - A[1] * pBoxScale[1] - A[2] * pBoxScale[2];
	vertex[6] = pBoxPos - A[0] * pBoxScale[0] + A[1] * pBoxScale[1] - A[2] * pBoxScale[2];
	vertex[7] = pBoxPos - A[0] * pBoxScale[0] - A[1] * pBoxScale[1] + A[2] * pBoxScale[2];

	INTERVAL result;
	result.fmin = result.fmax = D3DXVec3Dot(&axis, &vertex[0]);

	for (int i = 1; i < 8; ++i)
	{
		_float projection = D3DXVec3Dot(&axis, &vertex[i]);
		result.fmin = (projection < result.fmin) ?
			projection : result.fmin;
		result.fmax = (projection > result.fmax) ?
			projection : result.fmax;
	}

	return result;

}

Engine::_vec3 Engine::CCollisionMgr::GetMin(CSphereCollision* aabb)
{
	_vec3 aabbSize = 0.5f * _vec3(aabb->getRadius(), aabb->getRadius(), aabb->getRadius());
	_vec3 p1 = aabb->getCenter() + aabbSize;
	_vec3 p2 = aabb->getCenter() - aabbSize;

	return _vec3(fminf(p1.x, p2.x), fminf(p1.y, p2.y), fminf(p1.z, p2.z));
}

Engine::_vec3 Engine::CCollisionMgr::GetMax(CSphereCollision* aabb)
{
	_vec3 aabbSize = 0.5f * _vec3(aabb->getRadius(), aabb->getRadius(), aabb->getRadius());
	_vec3 p1 = aabb->getCenter() + aabbSize;
	_vec3 p2 = aabb->getCenter() - aabbSize;

	return _vec3(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y), fmaxf(p1.z, p2.z));
}


Engine::_bool Engine::CCollisionMgr::OverlapOnAixs(CSphereCollision* AABB, CBoxCollision* OBB, const _vec3& axis)
{
	INTERVAL a = GetInterval(AABB, axis);
	INTERVAL b = GetInterval(OBB, axis);
	return ((b.fmin <= a.fmax) && (a.fmin <= b.fmax));
}



Engine::_matrix Engine::CCollisionMgr::setAngletomatrix(const _vec3& angle)
{
	_matrix matSRP;
	D3DXMatrixIdentity(&matSRP);

	D3DXQUATERNION quatSRP;
	D3DXQuaternionIdentity(&quatSRP);

	D3DXQuaternionRotationYawPitchRoll(&quatSRP, D3DXToRadian(angle.y), D3DXToRadian(angle.x), D3DXToRadian(angle.z));
	D3DXMatrixRotationQuaternion(&matSRP, &quatSRP);

	return matSRP;
}

Engine::_bool Engine::CCollisionMgr::BoxtoBoxCollisionCheckAABBtoOBB(CCollision* pCol, CCollision* pCollider,_vec3* pVec,_vec3* walkpower)
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

	_matrix o;
	o = setAngletomatrix(pBox->getAngle());
	//pBox->getRotateMatrix(o);

	_vec3 Test[15] = {
		_vec3(1,0,0),
		_vec3(0,1,0),
		_vec3(0,0,1),
		_vec3(o.m[0][0],o.m[0][1],o.m[0][2]),
		_vec3(o.m[1][0],o.m[1][1],o.m[1][2]),
		_vec3(o.m[2][0],o.m[2][1],o.m[2][2])
	};

	for (int i = 0; i < 3; ++i)
	{
		D3DXVec3Cross(&Test[6 + i * 3 + 0], &Test[i], &Test[0]);
		D3DXVec3Cross(&Test[6 + i * 3 + 1], &Test[i], &Test[1]);
		D3DXVec3Cross(&Test[6 + i * 3 + 2], &Test[i], &Test[2]);
	}

	for (int i = 0; i < 15; ++i)
	{
		//�Ѱ��� �������Ǵ� ���� ���ٸ� �浹�̾ƴ�
		if (!OverlapOnAixs(pSphere, pBox, Test[i]))
		{
			return false;
		}
	}

	_vec3 vnormalz;  
	D3DXVec3Cross(&vnormalz, &Test[3], &Test[4]);
	_vec3 vnormalx;
	D3DXVec3Cross(&vnormalx, &Test[4], &Test[5]);

	_vec3 T1;
	D3DXVec3Cross(&T1, pVec, &vnormalz);
	_vec3 Out1;
	D3DXVec3Cross(&Out1, &T1, &vnormalz);


	_vec3 T2;
	D3DXVec3Cross(&T2, pVec, &vnormalx);
	_vec3 Out2;
	D3DXVec3Cross(&Out2, &T2, &vnormalx);

	if (D3DXVec3Dot(pVec, &vnormalx) < 0 && D3DXVec3Dot(pVec,&vnormalz) >=0)
	{
		*walkpower += Out1;
		//*walkpower -= Out2;
	}
	else if (D3DXVec3Dot(pVec, &vnormalx) < 0 && D3DXVec3Dot(pVec, &vnormalz) < 0)
	{
		*walkpower += Out1;

	}
	else if (D3DXVec3Dot(pVec, &vnormalx) >= 0 && D3DXVec3Dot(pVec, &vnormalz) >= 0)
	{
		*walkpower += Out1;

	}
	else if (D3DXVec3Dot(pVec, &vnormalx) >= 0 && D3DXVec3Dot(pVec,&vnormalz) < 0)
	{
		*walkpower += Out1;
	}


	/*if (D3DXVec3Dot(pVec, &vnormalz) >= 0)
	{
		*walkpower += -(*pVec - (D3DXVec3Dot(pVec, &vnormalz) * vnormalz));
		*walkpower += (*pVec - (D3DXVec3Dot(pVec, &vnormalx) * vnormalx));
	}
	else if (D3DXVec3Dot(pVec, &vnormalz) < 0)
	{
		*walkpower += (*pVec - (D3DXVec3Dot(pVec, &vnormalz) * vnormalz));
		*walkpower += (*pVec - (D3DXVec3Dot(pVec, &vnormalx) * vnormalx));

	}*/
	/*if(D3DXVec3Dot(pVec,&vnormalx) < 0)
	*walkpower += -(*pVec -  (D3DXVec3Dot(pVec, &vnormalx) * vnormalx) );*/

	std::cout << D3DXVec3Dot(pVec, &vnormalz) << std::endl;
	std::cout << D3DXVec3Dot(pVec, &vnormalx) << std::endl;

	return true;
}
