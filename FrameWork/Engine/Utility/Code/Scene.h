#pragma once
#ifndef __SCENE_H__
#define __SCENE_H__
#include "Base.h"
BEGIN(Engine)
class CLayer;
class CComponent;
class CGameObject;
class ENGINE_DLL CScene abstract : public CBase
{
protected:
	explicit CScene();
	explicit CScene(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CScene();
public:
	virtual HRESULT Init_Scene();
	virtual _int Update_Scene(const _float& fDeltaTime);
	virtual void LateUpdate_Scene();
	virtual void Render_Scene() PURE;
	HRESULT Add_Object(LAYERID eLayerID,GAMEOBJECTID eObjID, CGameObject* pObj);
private:
	CLayer* Find_Layer(LAYERID eLayerID);
protected:
	virtual void Free();
public:
	CComponent* getComponent(LAYERID eLayerID, GAMEOBJECTID eObjID, COMPONENTID eComID, COMPONENTTYPE eType);
	CGameObject* getGameObject(LAYERID eLayerID, GAMEOBJECTID eObjID);
	CGameObject* getGameObject(GAMEOBJECTID eObjID);
	vector<CGameObject*>* getGameObjects(LAYERID eLayerID, GAMEOBJECTID eObjID);
public:
	HRESULT setLayer(LAYERID eLayerID,CLayer* pLayer);
protected:
	LPDIRECT3DDEVICE9 m_pDevice;
	map<LAYERID, CLayer*> m_mapLayer;
};
END
#endif