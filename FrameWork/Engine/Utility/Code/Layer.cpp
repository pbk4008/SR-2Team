#include "Engine_Include.h"
#include "Layer.h"
#include "GameObject.h"
#include "Component.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
}

HRESULT CLayer::Init_Layer()
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float& fDeltaTime)
{
	_int iExit = 0;
	for (auto& ObjVector : m_mapObject)
	{
		for (auto& Obj : ObjVector.second)
		{
			if (!Obj->getActive())
				continue;
			iExit = Obj->Update_GameObject(fDeltaTime);
		}
	}
	return iExit;
}

void CLayer::LateUpdate_Layer()
{
	for (auto& ObjVector : m_mapObject)
	{
		for (auto& Obj : ObjVector.second)
			Obj->LateUpdate_GameObject();
	}
}

HRESULT CLayer::Add_Object(GAMEOBJECTID eObjID, CGameObject* pObj)
{
	vector<CGameObject*>* pObjVec = Find_GameObject(eObjID);
	if (pObjVec)
		(*pObjVec).push_back(pObj);
	else
	{
		vector<CGameObject*> pGameObjectVec;
		pGameObjectVec.push_back(pObj);
		m_mapObject.emplace(eObjID, pGameObjectVec);
	}
	return S_OK;
}

vector<CGameObject*>* CLayer::Find_GameObject(GAMEOBJECTID eObjID)
{
	auto objVec = m_mapObject.find(eObjID);//objVectorArr√£±‚
	if (objVec == m_mapObject.end())
		return nullptr;
	
	return &((*objVec).second);
}


CGameObject* CLayer::Pooling(vector<CGameObject*>* pGameObejctArr, GAMEOBJECTID eObjID)
{
	for (auto& pObj : (*pGameObejctArr))
	{
		if (pObj->getActive())
		{
			if (eObjID != GAMEOBJECTID::SHURIKEN&& eObjID != GAMEOBJECTID::BOMB)
				return pObj;
			else
				continue;
		}
		else
		{
			pObj->setActive(true);
			pObj->ResetObject();
			return pObj;
		}
	}
	return nullptr;
}

CLayer* CLayer::Create()
{
	CLayer* pInstance = new CLayer;
	if (FAILED(pInstance->Init_Layer()))
		Safe_Release(pInstance);
	return pInstance;
}

void CLayer::Free()
{
	for (auto& objVector : m_mapObject)//Vector
	{
		for_each((objVector.second).begin(), (objVector.second).end(), DeleteObj);//Obj
		objVector.second.clear();
		objVector.second.shrink_to_fit();
	}
	m_mapObject.clear();
}

CComponent* CLayer::getComponent(GAMEOBJECTID eObjID, COMPONENTID eComponentID, COMPONENTTYPE eType)
{
	vector<CGameObject*>* pObj = Find_GameObject(eObjID);
	NULL_CHECK_RETURN(pObj, nullptr);


	return pObj->front()->getComponent(eComponentID, eType);
}

CGameObject* CLayer::getGameObject(GAMEOBJECTID eObj)
{
	auto ObjArr = Find_GameObject(eObj);
	NULL_CHECK_RETURN(ObjArr, nullptr);

	return ObjArr->front();
}
