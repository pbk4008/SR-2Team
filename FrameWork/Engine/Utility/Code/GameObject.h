#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__
#include "Base.h"
BEGIN(Engine)
class CComponent;
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject();
	explicit CGameObject(LPDIRECT3DDEVICE9 pDevice);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();
public:
	virtual HRESULT Init_GameObject() PURE;
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual void LateUpdate_GameObject() PURE;
	virtual CGameObject* Clone_GameObject() PURE;
private:
	CComponent* Find_Component(COMPONENTID eID,COMPONENTTYPE eType);
protected:
	virtual void Free();
public:
	CComponent* getComponent(COMPONENTID eID, COMPONENTTYPE eType);
private:
	LPDIRECT3DDEVICE9 m_pDevice;
	map<COMPONENTID, CComponent*> m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_END];
	_ulong m_dwIndex;
};
END
#endif