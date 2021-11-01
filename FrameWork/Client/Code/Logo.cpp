#include "pch.h"
#include "Logo.h"
#include "Layer.h"

CLogo::CLogo()
{
}

CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice) : CScene(pDevice)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Init_Scene()
{
	FAILED_CHECK_RETURN(Init_Layer(), E_FAIL);

	return S_OK;
}

_int CLogo::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CScene::Update_Scene(fDeltaTime);

	
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
