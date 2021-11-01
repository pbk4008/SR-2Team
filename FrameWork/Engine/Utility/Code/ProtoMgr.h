#pragma once
#ifndef __PROTOMGR_H__
#define __PROTOMGR_H__
#include "Base.h"
BEGIN(Engine)
class CComponent;
class CGameObject;
class ENGINE_DLL CProtoMgr final : public CBase
{
	DECLARE_SINGLETON(CProtoMgr)
private:
	explicit CProtoMgr();
	virtual ~CProtoMgr();
public:
	HRESULT Init_ComProto(COMPONENTID eID, CComponent* pComponent);
	HRESULT Init_ObjProto(GAMEOBJECTID eID, CGameObject* pGameObject);
	CComponent* Clone_ComProto(COMPONENTID eID);
	CGameObject* Clone_ObjProto(GAMEOBJECTID eID);
private:
	CComponent* Find_ComProto(COMPONENTID eID);
	CGameObject* Find_ObjProto(GAMEOBJECTID eID);
private:
	virtual void Free();
private:
	map<COMPONENTID, CComponent*> m_mapComProto;
	map<GAMEOBJECTID, CGameObject*> m_mapObjProto;
};
END
#endif