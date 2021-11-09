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
	_int Update_Collision(const _float& fDeltaTime);
	void Collision();
private:
	virtual void Free();
private:
	vector<CCollision*> m_vecCollision;
};
END
#endif