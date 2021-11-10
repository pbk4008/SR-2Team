#pragma once
#ifndef __COLLISIONMGR_H__
#define __COLLISIONMGR_H__
#include "Base.h"
BEGIN(Engine)
class CCollision;
class ENGINE_DLL CCollisionMgr final : public CBase
{
	DECLARE_SINGLETON(CCollisionMgr)
private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();
public:
	HRESULT Insert_Collision(CCollision* pCollision);
	void TerrainCollision(const _float& fX, _float& fY, const _float& fZ, 
		const _vec3* pTerrainVtxPos,
		const _ulong& dwCntX,
		const _ulong& dwCntZ,
		const _ulong& dwVtxItv);
	void Collision(CCollision* pCollision , COLLISIONTAG eTag);
	void ClearCollisionList();
private:
	_bool CollisionCheck(CCollision* pCol, CCollision* pCollider);
private:
	virtual void Free();
private:
	vector<CCollision*> m_vecCollision;
};
END
#endif