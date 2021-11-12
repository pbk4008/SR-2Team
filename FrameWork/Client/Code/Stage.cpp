#include "pch.h"
#include "Stage.h"
#include "Layer.h"
#include "Loading.h"
#include "Monster.h"
#include "MeleeMon.h"
#include "Player.h"
#include "MainCamera.h"
#include "PlayerModel.h"
#include "Terrain.h"
#include "ShootMon.h"
#include "FlyMon.h"

CStage::CStage() : m_pLoading(nullptr)
{
}

CStage::CStage(LPDIRECT3DDEVICE9 pDevice) : CScene(pDevice), m_pLoading(nullptr)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Init_Scene()
{
	FAILED_CHECK_RETURN(Init_Layer(), E_FAIL);

	m_pLoading = CLoading::Create(m_pDevice, SCENEID::STAGE_TWO);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

_int CStage::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CScene::Update_Scene(fDeltaTime);

	return iExit;
}

void CStage::LateUpdate_Scene()
{
	CScene::LateUpdate_Scene();
}

void CStage::Render_Scene()
{
	
}

HRESULT CStage::Init_Layer()
{
	FAILED_CHECK_RETURN(Init_Environment_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Init_GameLogic_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Init_UI_Layer(), E_FAIL);

	return S_OK;
}

HRESULT CStage::Init_Environment_Layer()
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	//TODO : Environment게임오브젝트 추가
	CGameObject* pGameObject = nullptr;

	pGameObject = Clone_ObjProto<CTerrain>(GAMEOBJECTID::TERRAIN);
	pLayer->Add_Object(GAMEOBJECTID::TERRAIN, pGameObject);

	m_mapLayer.emplace(LAYERID::ENVIRONMENT, pLayer);

	return S_OK;
}

HRESULT CStage::Init_GameLogic_Layer()
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	//TODO : GameLogic 게임오브젝트 추가

	CGameObject* pGameObject = nullptr;
	//Player생성

	CPlayer* pPlayer = nullptr;

	pGameObject = pPlayer = Clone_ObjProto<CPlayer>(GAMEOBJECTID::PLAYER);

	CMainCamera* pCam = Clone_ObjProto<CMainCamera>(GAMEOBJECTID::CAMERA);
	CPlayerModel* pModel = Clone_ObjProto<CPlayerModel>(GAMEOBJECTID::PLAYERMODEL);

	pPlayer->setModel(pModel);
	pPlayer->setCamera(pCam);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::PLAYER, pGameObject), E_FAIL);

	// meleemon
	CMeleeMon* m_pMeleeMon = nullptr;
	pGameObject = m_pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::MONSTER1, pGameObject), E_FAIL);
	
	//// shootmon
	/*CShootMon* m_pMeleeMon = nullptr;
	pGameObject = m_pMeleeMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::MONSTER2, pGameObject), E_FAIL);*/

	//// flymon
	/*CFlyMon* m_pMeleeMon = nullptr;
	pGameObject = m_pMeleeMon = Clone_ObjProto<CFlyMon>(GAMEOBJECTID::MONSTER3);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::MONSTER3, pGameObject), E_FAIL);*/

	m_mapLayer.emplace(LAYERID::GAME_LOGIC, pLayer);
	return S_OK;
}

HRESULT CStage::Init_UI_Layer()
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	//TODO : UI 게임오브젝트 추가
	CGameObject* pGameObject = nullptr;

	m_mapLayer.emplace(LAYERID::UI, pLayer);
	return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage* pInstance = new CStage(pDevice);
	if (FAILED(pInstance->Init_Scene()))
		Safe_Release(pInstance);
	return pInstance;
}

void CStage::Free()
{
	CRenderer::GetInstance()->Clear_RenderList();
	Safe_Release(m_pLoading);
	CScene::Free();
}
