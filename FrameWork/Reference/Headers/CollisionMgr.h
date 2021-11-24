#pragma once
#ifndef __COLLISIONMGR_H__
#define __COLLISIONMGR_H__
#include "Base.h"
BEGIN(Engine)
class CCollision;
class ENGINE_DLL CCollisionMgr final : public CBase
{
private:
	typedef struct tagAABB
	{
		_float fMinX;
		_float fMaxX;
		_float fMinY;
		_float fMaxY;
		_float fMinZ;
		_float fMaxZ;
	}AABB;
	DECLARE_SINGLETON(CCollisionMgr)
private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();
public:
	HRESULT Insert_Collision(CCollision* pCollision);
	HRESULT Insert_ObjCollision(CCollision* pCollision);
	HRESULT Insert_Wall(CCollision* pCollision);
	void TerrainCollision(const _float& fX, _float& fY, const _float& fZ, 
		const _vec3* pTerrainVtxPos,
		const _ulong& dwCntX,
		const _ulong& dwCntZ,
		const _ulong& dwVtxItv
		,const _float& fTerrinsYPos);
	void Collision(CCollision* pCollision , COLLISIONTAG eTag);
	void ClearCollision();
	void ClearWall();
	void WallCollision(CCollision* pCollsion, _vec3& MoveVec);
	void CopyCollision();
private:
	_bool SphereCollisionCheck(CCollision* pCol, CCollision* pCollider);
	_bool BoxCollisionCheck(CCollision* pCol, CCollision* pCollider);
	_bool BoxToSphereCollisionCheck(CCollision* pCol, CCollision* pCollider, _vec3* pVec = nullptr);
	_bool ShpereBoxCollisionCheck(CCollision* pCol, CCollision* pCollider, _vec3* pVec = nullptr);
	_bool BoxtoSphereCollisionCheckOBB(CCollision* pCol, CCollision* pCollider);
private:
	virtual void Free();
public:
	vector<CCollision*> getVecWall() { return m_vecWall; }
private:
	vector<CCollision*> m_vecCollision;
	vector<CCollision*> m_vecWall;

	vector<CCollision*> m_vecLoadingCollision;
	vector<CCollision*> m_vecLoadingWall;
};
END
#endif