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

_int CCollisionMgr::Update_Collision(const _float& fDeltaTime)
{
	_int iExit = 0;
	for (auto& pCol : m_vecCollision)
		iExit=pCol->Update_Component(fDeltaTime);
	return iExit;
}

void CCollisionMgr::Collision()
{
	
}

void CCollisionMgr::Free()
{
}
