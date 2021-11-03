#include "pch.h"
#include "Logo.h"
#include "Layer.h"
#include "Loading.h"
CLogo::CLogo():m_pLoading(nullptr)
{
}

CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice) : CScene(pDevice), m_pLoading(nullptr)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Init_Scene()
{
	FAILED_CHECK_RETURN(Init_ProtoMgr(), E_FAIL);
	FAILED_CHECK_RETURN(Init_Layer(), E_FAIL);

	m_pLoading = CLoading::Create(m_pDevice, SCENEID::STAGE_ONE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

_int CLogo::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = 0;
	if (m_pLoading->getFinish())
	{
		//ToDo:로딩 씬에서 다음씬으로 넘어가는 상호작용
		//if (Key_Down(VIR_ENTER))
		//{
		//	/*CScene* pScene = nullptr;

		//	pScene = CStage::Create(m_pDevice);
		//	NULL_CHECK_RETURN(pScene, E_FAIL);

		//	FAILED_CHECK_RETURN(Change_Scene(pScene), E_FAIL);*/

		//	return iExit;
		//}

	}
	return iExit;
}

void CLogo::LateUpdate_Scene()
{
	CScene::LateUpdate_Scene();
}

void CLogo::Render_Scene()
{
	//Debug용
}

HRESULT CLogo::Init_Layer()
{
	FAILED_CHECK_RETURN(Init_Environment_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Init_GameLogic_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Init_UI_Layer(), E_FAIL);

	return S_OK;
}

HRESULT CLogo::Init_Environment_Layer()
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	//TODO : Environment게임오브젝트 추가
	CGameObject* pGameObject = nullptr;

	m_mapLayer.emplace(LAYERID::ENVIRONMENT, pLayer);
	return S_OK;
}

HRESULT CLogo::Init_GameLogic_Layer()
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	//TODO : GameLogic 게임오브젝트 추가
	CGameObject* pGameObject = nullptr;

	m_mapLayer.emplace(LAYERID::GAME_LOGIC, pLayer);

	return S_OK;
}

HRESULT CLogo::Init_UI_Layer()
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	//TODO : UI 게임오브젝트 추가
	CGameObject* pGameObject = nullptr;

	m_mapLayer.emplace(LAYERID::UI, pLayer);
	return S_OK;
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogo* pInstance = new CLogo(pDevice);
	if (FAILED(pInstance->Init_Scene()))
		Safe_Release(pInstance);
	return pInstance;
}

void CLogo::Free()
{
	CScene::Free();
}
