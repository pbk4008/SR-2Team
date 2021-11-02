#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__
#include "Base.h"
BEGIN(Engine)
class CComponent;
class CTransform;
class CProtoMgr;
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
	virtual void Render_GameObject() PURE;
	virtual CGameObject* Clone_GameObject() PURE;
protected:
	CComponent* Find_Component(COMPONENTID eID,COMPONENTTYPE eType);
protected:
	virtual void Add_Component();
	virtual void Free();
public:
	CComponent* getComponent(COMPONENTID eID, COMPONENTTYPE eType);
	inline _bool getActive() { return m_bActive; }
	inline CTransform* getTransform() { return m_pTransform; }
public:
	inline void setActive(const _bool& bActive) { m_bActive = bActive; }
private:
	LPDIRECT3DDEVICE9 m_pDevice;
	map<COMPONENTID, CComponent*> m_mapComponent[(_ulong)COMPONENTTYPE::TYPE_END];
	_bool m_bActive;//게임 오브젝트 활성화 및 비활성화 상태 판단
	CProtoMgr* m_pProtoMgr;
	CTransform* m_pTransform;
};
END
#endif