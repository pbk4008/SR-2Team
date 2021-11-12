#include "Engine_Include.h"
#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"

CScene::CScene() : m_pDevice(nullptr)
{
}

CScene::CScene(LPDIRECT3DDEVICE9 pDevice) : m_pDevice(pDevice)
{
	m_pDevice->AddRef();
}

CScene::~CScene()
{
}

HRESULT CScene::Init_Scene()
{
	return S_OK;
}

_int CScene::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = 0;
	for (auto& iter : m_mapLayer)
	{
		iExit = iter.second->Update_Layer(fDeltaTime);
		if (iExit & 0x80000000)
			return iExit;
	}
	return iExit;
}

void CScene::LateUpdate_Scene()
{
	for (auto& iter : m_mapLayer)
		iter.second->LateUpdate_Layer();
}

HRESULT CScene::Add_Object(LAYERID eLayerID, GAMEOBJECTID eObjID, CGameObject* pObj)
{
	CLayer* pLayer = Find_Layer(eLayerID);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	pLayer->Add_Object(eObjID, pObj);
	return S_OK;
}

CLayer* CScene::Find_Layer(LAYERID eLayerID)
{
	auto pLayer = m_mapLayer.find(eLayerID);
	if (pLayer == m_mapLayer.end())
		return nullptr;

	return pLayer->second;
}

void CScene::Free()
{
	Safe_Release(m_pDevice);
	for_each(m_mapLayer.begin(), m_mapLayer.end(), DeleteMap);
	m_mapLayer.clear();
}

CComponent* CScene::getComponent(LAYERID eLayerID, GAMEOBJECTID eObjID, COMPONENTID eComID, COMPONENTTYPE eType)
{
	CLayer* pLayer = Find_Layer(eLayerID);

	NULL_CHECK_RETURN(pLayer, nullptr);

	return pLayer->getComponent(eObjID, eComID, eType);
}

CGameObject* CScene::getGameObject(LAYERID eLayerID, GAMEOBJECTID eObjID)
{
	CGameObject* pGameObject = nullptr;
	auto iter = Find_Layer(eLayerID);
	NULL_CHECK_RETURN(iter, nullptr);

	return iter->getGameObject(eObjID);
}

CGameObject* CScene::getGameObject(GAMEOBJECTID eObjID)
{
	CGameObject* pGameObject = nullptr;
	for (auto& iter : m_mapLayer)
	{
		if (pGameObject = (iter.second)->getGameObject(eObjID))
			return pGameObject;
	}
	return nullptr;
}
