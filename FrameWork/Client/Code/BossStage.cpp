#include "pch.h"
#include "BossStage.h"
#include "Layer.h"
#include "Loading.h"
#include "Player.h"
#include "MainCamera.h"
#include "PlayerModel.h"
#include "Terrain.h"
#include "Boss.h"
#include "HP.h"
#include "UI.h"

CBossStage::CBossStage() : m_pLoading(nullptr), m_pPlayer(nullptr)
{
}

CBossStage::CBossStage(LPDIRECT3DDEVICE9 pDevice) : CScene(pDevice), m_pLoading(nullptr)
, m_pPlayer(nullptr)

{
}

CBossStage::~CBossStage()
{
}

HRESULT CBossStage::Init_Scene()
{
	FAILED_CHECK_RETURN(Init_Layer(), E_FAIL);
	
	return S_OK;
}

_int CBossStage::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CScene::Update_Scene(fDeltaTime);

	/*CBoss* pBoss = nullptr;
	pBoss = Clone_ObjProto<CBoss>(GAMEOBJECTID::BOSS);
	_vec3 vPos = { 5.f,1.f,5.f };
	_vec3 vScale = { 1.f,1.f,1.f };
	_vec3 vAngle = { 0.f,0.f,0.f };
	pBoss->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::BOSS, pBoss);

	CGameObject* pGameObject = nullptr;
	CHP* pHP = nullptr;
	pGameObject = pHP = CHP::Create(m_pDevice);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::BOSSHP, pBoss);*/

	return iExit;
}

void CBossStage::LateUpdate_Scene()
{
	CScene::LateUpdate_Scene();
}

void CBossStage::Render_Scene()
{
}

HRESULT CBossStage::Init_Layer()
{
	FAILED_CHECK_RETURN(Init_Environment_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Init_GameLogic_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Init_UI_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Init_Loading_Layer(), E_FAIL);

	return S_OK;
}

HRESULT CBossStage::Init_Environment_Layer()
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	//TODO : Environment게임오브젝트 추가

	m_mapLayer.emplace(LAYERID::ENVIRONMENT, pLayer);

	return S_OK;
}

HRESULT CBossStage::Init_GameLogic_Layer()
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	//TODO : GameLogic 게임오브젝트 추가

	CGameObject* pGameObject = nullptr;
	//Player생성

	pGameObject = m_pPlayer = Clone_ObjProto<CPlayer>(GAMEOBJECTID::PLAYER);

	CMainCamera* pCam = Clone_ObjProto<CMainCamera>(GAMEOBJECTID::CAMERA);
	CPlayerModel* pModel = Clone_ObjProto<CPlayerModel>(GAMEOBJECTID::PLAYERMODEL);

	m_pPlayer->setModel(pModel);
	m_pPlayer->setCamera(pCam);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::PLAYER, pGameObject), E_FAIL);
	m_pPlayer->AddRef();

	//boss
	CBoss* m_pBoss = nullptr;
	pGameObject = m_pBoss = Clone_ObjProto<CBoss>(GAMEOBJECTID::BOSS);
	_vec3 vPos = { 50.f,1.f,50.f };
	_vec3 vScale = { 1.f,1.f,1.f };
	_vec3 vAngle = { 0.f,0.f,0.f };
	m_pBoss->LoadTransform(vScale, vAngle, vPos);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::BOSS, pGameObject), E_FAIL);

	CHP* pHP = nullptr;
	pGameObject = pHP = CHP::Create(m_pDevice);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::BOSSHP, pGameObject), E_FAIL);

	m_mapLayer.emplace(LAYERID::GAME_LOGIC, pLayer);
	return S_OK;
}

HRESULT CBossStage::Init_UI_Layer()
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	//TODO : UI 게임오브젝트 추가
	CGameObject* pGameObject = nullptr;

	CUI* pUI = nullptr;
	pGameObject = CUI::Create(m_pDevice);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::UI, pGameObject), E_FAIL);

	m_mapLayer.emplace(LAYERID::UI, pLayer);
	return S_OK;
}

HRESULT CBossStage::Init_Loading_Layer()
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	CGameObject* pGameObject = nullptr;

	m_mapLayer.emplace(LAYERID::LOADING, pLayer);
	return S_OK;
}


CBossStage* CBossStage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBossStage* pInstance = new CBossStage(pDevice);
	if (FAILED(pInstance->Init_Scene()))
		Safe_Release(pInstance);
	return pInstance;
}

void CBossStage::Free()
{
	ClearWall();
	ClearCollision();
	CRenderer::GetInstance()->Clear_RenderList();
	Safe_Release(m_pLoading);
	CScene::Free();

	Safe_Release(m_pPlayer);

}
