#include "Engine_Include.h"
#include "Management.h"
#include "Scene.h"
#include "Component.h"

IMPLEMENT_SINGLETON(CManagement)
CManagement::CManagement() : m_pScene(nullptr)
{

}
CManagement::~CManagement()
{
}

HRESULT CManagement::Change_Scene(CScene* pScene)
{
	Safe_Release(m_pScene);
	
	m_pScene = pScene;
	return S_OK;
}

_int CManagement::Update_Management(const _float& fDeltaTime)
{
	NULL_CHECK_RETURN(m_pScene, -1);

	_int iExit = m_pScene->Update_Scene(fDeltaTime);
	return iExit;
}

void CManagement::LateUpdate_Management()
{
	NULL_CHECK(m_pScene);

	m_pScene->LateUpdate_Scene();
}

void CManagement::Render_Manangement(LPDIRECT3DDEVICE9& pDevice)
{
	NULL_CHECK(m_pScene);

	m_pScene->Render_Scene();
}

void CManagement::Free()
{
	Safe_Release(m_pScene);
}

CComponent* CManagement::getComponent(LAYERID eLayerID, GAMEOBJECTID eObjID, COMPONENTID eComID, COMPONENTTYPE eType)
{
	NULL_CHECK_RETURN(m_pScene, nullptr);

	return m_pScene->getComponent(eLayerID, eObjID, eComID, eType);
}
