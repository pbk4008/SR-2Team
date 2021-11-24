#include "Engine_Include.h"
#include "ProtoMgr.h"
#include "Component.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CProtoMgr)
CProtoMgr::CProtoMgr()
{

}
CProtoMgr::~CProtoMgr()
{
}

//Component원본저장
HRESULT CProtoMgr::Init_ComProto(COMPONENTID eID, CComponent* pComponent)
{
	CComponent* pComProto = Find_ComProto(eID);
	if (pComProto)
		return E_FAIL;

	m_mapComProto.emplace(eID, pComponent);
	return S_OK;
}

//GameObject 원본저장
HRESULT CProtoMgr::Init_ObjProto(GAMEOBJECTID eID, CGameObject* pGameObject)
{
	CGameObject* pObjProto = Find_ObjProto(eID);
	if (pObjProto)
		return E_FAIL;

	m_mapObjProto.emplace(eID, pGameObject);
	return S_OK;
}
_bool CProtoMgr::checkObject(GAMEOBJECTID eID)
{
	CGameObject* pObj = Find_ObjProto(eID);
	if (pObj)
		return true;
	return false;
}
_bool CProtoMgr::checkComponent(COMPONENTID eID)
{
	CComponent* pObj = Find_ComProto(eID);
	if (pObj)
		return true;
	return false;
}
CComponent* CProtoMgr::Find_ComProto(COMPONENTID eID)
{
	auto iter = m_mapComProto.find(eID);

	if (iter == m_mapComProto.end())
		return nullptr;

	return iter->second;
}

CGameObject* CProtoMgr::Find_ObjProto(GAMEOBJECTID eID)
{
	auto iter = m_mapObjProto.find(eID);

	if (iter == m_mapObjProto.end())
		return nullptr;

	return iter->second;
}

void CProtoMgr::Free()
{
	for_each(m_mapObjProto.begin(), m_mapObjProto.end(), DeleteMap);
	m_mapObjProto.clear();
	for_each(m_mapComProto.begin(), m_mapComProto.end(), DeleteMap);
	m_mapComProto.clear();
}

_bool CProtoMgr::checkObject(GAMEOBJECTID eID)
{
	CGameObject* pObj = Find_ObjProto(eID);
	if (pObj)
		return true;
	return false;
}

_bool CProtoMgr::checkComponent(COMPONENTID eID)
{
	CComponent* pObj = Find_ComProto(eID);
	if (pObj)
		return true;
	return false;
}