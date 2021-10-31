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
		for(auto& Obj : ObjVector.second)
			iExit = Obj->Update_GameObject(fDeltaTime);
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

CGameObject* CLayer::Find_GameObject(GAMEOBJECTID eObjID, COMPONENTID eComponentID)
{
	auto objVec = m_mapObject.find(eObjID);//objVectorArrÃ£±â
	if (objVec == m_mapObject.end())
		return nullptr;
	
	return (*objVec).second.front();

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
		for_each(objVector.second.begin(), objVector.second.end(), DeleteObj);//Obj
		objVector.second.clear();
		objVector.second.shrink_to_fit();
	}
	m_mapObject.clear();
}

CComponent* CLayer::getComponent(GAMEOBJECTID eObjID, COMPONENTID eComponentID, COMPONENTTYPE eType)
{
	CGameObject* pObj = Find_GameObject(eObjID, eComponentID);
	NULL_CHECK_RETURN(pObj, nullptr);

	return pObj->getComponent(eComponentID, eType);
}
