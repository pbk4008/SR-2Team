#pragma once
#ifndef __PROTOMGR_H__
#define __PROTOMGR_H__
#include "Component.h"
#include "GameObject.h"
#include "Base.h"
BEGIN(Engine)
class ENGINE_DLL CProtoMgr final : public CBase
{
	DECLARE_SINGLETON(CProtoMgr)
private:
	explicit CProtoMgr();
	virtual ~CProtoMgr();
public:
	HRESULT Init_ComProto(COMPONENTID eID, CComponent* pComponent);
	HRESULT Init_ObjProto(GAMEOBJECTID eID, CGameObject* pGameObject);
	_bool checkObject(GAMEOBJECTID eID);
	_bool checkComponent(COMPONENTID eID);
public:
	template<typename T>
	T* Clone_ComProto(COMPONENTID eID)//캐스팅 된 상태로 바로 받기 위해서 템플릿 함수 사용
	{
		CComponent* pComProto = Find_ComProto(eID);
		NULL_CHECK_RETURN(pComProto, nullptr);

		return static_cast<T*>(pComProto->Clone_Component());
	}
	template<typename T>
	T* Clone_ObjProto(GAMEOBJECTID eID)//캐스팅 된 상태로 바로 받기 위해서 템플릿 함수 사용
	{
		CGameObject* pObjProto = Find_ObjProto(eID);
		NULL_CHECK_RETURN(pObjProto, nullptr);

		return static_cast<T*>(pObjProto->Clone_GameObject());
	}

public:
	_bool checkObject(GAMEOBJECTID eID);
	_bool checkComponent(COMPONENTID eID);

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