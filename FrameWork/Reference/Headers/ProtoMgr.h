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
public:
	template<typename T>
	T* Clone_ComProto(COMPONENTID eID)//ĳ���� �� ���·� �ٷ� �ޱ� ���ؼ� ���ø� �Լ� ���
	{
		CComponent* pComProto = Find_ComProto(eID);
		NULL_CHECK_RETURN(pComProto, nullptr);

		return static_cast<T*>(pComProto->Clone_Component());
	}
	template<typename T>
	T* Clone_ObjProto(GAMEOBJECTID eID)//ĳ���� �� ���·� �ٷ� �ޱ� ���ؼ� ���ø� �Լ� ���
	{
		CGameObject* pObjProto = Find_ObjProto(eID);
		NULL_CHECK_RETURN(pObjProto, nullptr);

		return static_cast<T*>(pObjProto->Clone_GameObject());
	}
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