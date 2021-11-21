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

vector<CGameObject*>* CScene::getGameObjects(LAYERID eLayerID, GAMEOBJECTID eObjID)
{
	auto iter = Find_Layer(eLayerID);
	NULL_CHECK_RETURN(iter, nullptr);

	return iter->getGameObjects(eObjID);
}

HRESULT CScene::setLayer(LAYERID eLayerID, CLayer* pLayer)
{
	auto iter = m_mapLayer.find(eLayerID);
	if (iter == m_mapLayer.end())
		m_mapLayer.emplace(eLayerID, pLayer);
	pLayer->AddRef();
	Safe_Release((*iter).second);
	(*iter).second = pLayer;

	CLayer* pEnvironmentLayer = m_mapLayer[LAYERID::ENVIRONMENT];
	vector<GAMEOBJECTID> pIDVec;
	vector<vector<CGameObject*>> pObjVec;
	(*iter).second->getAllObjecID(pIDVec);
	(*iter).second->getAllObject(pObjVec);

	_int iSize = pIDVec.size();
	for (_int i = 0; i < iSize; i++)
	{
		for (auto pObj : pObjVec[i])
		{
			pEnvironmentLayer->Add_Object(pIDVec[i], pObj);
			pObj->AddRef();
			pObj->setActive(true);
		}
	}

	(*iter).second->DeleteLayer();
	return S_OK;
}
