#pragma once
#ifndef __LAYER_H__
#define __LAYER_H__
#include "Base.h"
BEGIN(Engine)
class CGameObject;
class CComponent;
class ENGINE_DLL CLayer final : public CBase
{
private:
	explicit CLayer();
	virtual~CLayer();
public:
	HRESULT Init_Layer();
	_int Update_Layer(const _float& fDeltaTime);
	void LateUpdate_Layer();
	HRESULT Add_Object(GAMEOBJECTID eObjID, CGameObject* pObj);
private:
	vector<CGameObject*>* Find_GameObject(GAMEOBJECTID eObjID);
public:
	static CLayer* Create();
private:
	virtual void Free();
public:
	CComponent* getComponent(GAMEOBJECTID eObjID,COMPONENTID eComponentID, COMPONENTTYPE eType);
	CGameObject* getGameObject(GAMEOBJECTID eObj);
private:
	map<GAMEOBJECTID, vector<CGameObject*>> m_mapObject;
};
END
#endif