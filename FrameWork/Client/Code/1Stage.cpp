#include "pch.h"
#include "1Stage.h"
#include "Layer.h"
#include "Loading.h"
#include "MeleeMon.h"
#include "Player.h"
#include "MainCamera.h"
#include "PlayerModel.h"
#include "ShootMon.h"
#include "FlyMon.h"
#include "UI.h"
#include "Door.h"
#include "Spawner.h"
#include "DoorObserver.h"

C1Stage::C1Stage() : m_pLoading(nullptr), m_bFloorClear(false), m_bFirst(false), m_pPlayer(nullptr), m_pSpawner(nullptr)
{
	m_vecDoor.reserve(3);
	m_vecDoorObserver.reserve(3);
	m_vecClearBox.reserve(9);
}

C1Stage::C1Stage(LPDIRECT3DDEVICE9 pDevice) : CScene(pDevice), m_pLoading(nullptr), m_bFloorClear(false), m_bFirst(false)
, m_pPlayer(nullptr), m_pSpawner(nullptr)
{
	m_vecDoorObserver.reserve(3);
	m_vecDoor.reserve(3);
	m_vecClearBox.reserve(9);
}

C1Stage::~C1Stage()
{
}

HRESULT C1Stage::Init_Scene()
{
	FAILED_CHECK_RETURN(Init_Layer(), E_FAIL);

	m_pLoading = CLoading::Create(m_pDevice, SCENEID::STAGE_TWO);
	
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

_int C1Stage::Update_Scene(const _float& fDeltaTime)
{
	if (!m_bFirst)
	{
		m_bFirst = true;
		setClearBox();
	}
	_int iExit = 0;
	iExit = CScene::Update_Scene(fDeltaTime);

	for (auto pObserver : m_vecDoorObserver)
		pObserver->ActiveObserver();

	if (m_pPlayer->getKeyCount() > 5)//1번룸(아이템 획득)
		m_vecDoorObserver[0]->ClearObserver();
	if (m_vecDoorObserver[1]->getSpawnerActive())//2번룸(버티기 35마리 나옴)
		m_vecDoorObserver[1]->ClearObserver();
	if (m_vecDoorObserver[2]->getSpawnerActive())//3번룸(모든 포탈 닫기)
		m_vecDoorObserver[2]->ClearObserver();

	for (auto pDoor : m_vecDoor)
	{
		if (!pDoor->getClear())
		{
			m_bFloorClear = false;
			break;
		}
		else
			m_bFloorClear = true;
	}
	if (m_bFloorClear)
		FloorClear();
	if (m_pPlayer->getJumpCount() == 0)
	{
		//게임종료
	}
	return iExit;
}

void C1Stage::LateUpdate_Scene()
{
	CScene::LateUpdate_Scene();
}

void C1Stage::Render_Scene()
{
}

HRESULT C1Stage::Init_Layer()
{
	FAILED_CHECK_RETURN(Init_Environment_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Init_GameLogic_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Init_UI_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Init_Loading_Layer(), E_FAIL);

	return S_OK;
}

HRESULT C1Stage::Init_Environment_Layer()
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	//TODO : Environment게임오브젝트 추가

	m_mapLayer.emplace(LAYERID::ENVIRONMENT, pLayer);

	return S_OK;
}

HRESULT C1Stage::Init_GameLogic_Layer()
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

	for (_int i = 0; i < 3; i++)
	{
		CDoor* pDoor = Clone_ObjProto<CDoor>(GAMEOBJECTID::DOOR);
		pDoor->AddRef();
		pDoor->setIndex(m_vecDoor.size());
		m_vecDoor.emplace_back(pDoor);
		FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::DOOR,pDoor),E_FAIL);

		CDoorObserver* pDoorObserver = CDoorObserver::Create();
		pDoorObserver->setDoor(pDoor);
		m_vecDoorObserver.emplace_back(pDoorObserver);
	}
	DoorSetting();

	/*CMeleeMon* m_pMeleeMon = nullptr;
	pGameObject = m_pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::MONSTER1, pGameObject), E_FAIL);*/

	////CShootMon* m_pShootMonn = nullptr;
	//pGameObject = m_pShootMonn = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
	//FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::MONSTER2, pGameObject), E_FAIL);

	//CFlyMon* m_pFlyMonn = nullptr;
	//pGameObject = m_pFlyMonn = Clone_ObjProto<CFlyMon>(GAMEOBJECTID::MONSTER3);
	//FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::MONSTER3, pGameObject), E_FAIL);*/

	CSpawner* pSpawner = nullptr;

	pSpawner = Clone_ObjProto<CSpawner>(GAMEOBJECTID::SPAWNER);
	_vec3 vPos = { 15.f,1.f,80.f };
	_vec3 vAngle = { 0.f,0.f,0.f };
	pSpawner->setTransform(vPos, vAngle);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::SPAWNER, pSpawner), E_FAIL);
	pSpawner->setIndex(0);
	m_vecDoorObserver[0]->Insert_Spawner(pSpawner);

	pSpawner = Clone_ObjProto<CSpawner>(GAMEOBJECTID::SPAWNER);
	vPos = { 80.f,1.f,80.f };
	vAngle = { 0.f,90.f,0.f };
	pSpawner->setTransform(vPos, vAngle);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::SPAWNER, pSpawner), E_FAIL);
	pSpawner->setIndex(1);
	m_vecDoorObserver[1]->Insert_Spawner(pSpawner);

	pSpawner = Clone_ObjProto<CSpawner>(GAMEOBJECTID::SPAWNER);
	vPos = { 40.f,1.f,35.f };
	vAngle = {0.f,120.f,0.f };
	pSpawner->setTransform(vPos, vAngle);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::SPAWNER, pSpawner), E_FAIL);
	pSpawner->setIndex(2);
	pSpawner->SettingCollision();
	m_vecDoorObserver[2]->Insert_Spawner(pSpawner);
	
	pSpawner = Clone_ObjProto<CSpawner>(GAMEOBJECTID::SPAWNER);
	vPos = { 40.f,1.f,15.f };
	vAngle = { 0.f,30.f,0.f };
	pSpawner->setTransform(vPos, vAngle);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::SPAWNER, pSpawner), E_FAIL);
	pSpawner->setIndex(2);
	pSpawner->SettingCollision();
	m_vecDoorObserver[2]->Insert_Spawner(pSpawner);
	
	pSpawner = Clone_ObjProto<CSpawner>(GAMEOBJECTID::SPAWNER);
	vPos = { 60.f,1.f,15.f };
	vAngle = { 0.f,-30.f,0.f };
	pSpawner->setTransform(vPos, vAngle);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::SPAWNER, pSpawner), E_FAIL);
	pSpawner->setIndex(2);
	pSpawner->SettingCollision();
	m_vecDoorObserver[2]->Insert_Spawner(pSpawner);
	
	pSpawner = Clone_ObjProto<CSpawner>(GAMEOBJECTID::SPAWNER);
	vPos = { 60.f,1.f,35.f };
	vAngle = { 0.f,-120.f,0.f };
	pSpawner->setTransform(vPos, vAngle);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::SPAWNER, pSpawner), E_FAIL);
	pSpawner->setIndex(2);
	pSpawner->SettingCollision();
	m_vecDoorObserver[2]->Insert_Spawner(pSpawner);

	m_mapLayer.emplace(LAYERID::GAME_LOGIC, pLayer);
	return S_OK;
}

HRESULT C1Stage::Init_UI_Layer()
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

HRESULT C1Stage::Init_Loading_Layer()
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	CGameObject* pGameObject = nullptr;

	m_mapLayer.emplace(LAYERID::LOADING, pLayer);
	return S_OK;
}

void C1Stage::DoorSetting()
{
	_vec3 vScale, vRotate, vPos, vTrigger;
	ZeroMemory(&vScale, sizeof(_vec3));
	ZeroMemory(&vRotate, sizeof(_vec3));
	ZeroMemory(&vPos, sizeof(_vec3));

	vScale = { 9.f,7.f,1.f };
	vPos = { 27.f, 3.5f,65.f };
	vTrigger = { 20.f,1.f,68.f };
	m_vecDoor[0]->setTransform(vScale, vRotate, vPos);
	m_vecDoor[0]->setTrigger(vTrigger);

	vScale = { 1.f,7.f,9.f };
	vPos = { 63.f, 3.5f,72.f };
	vTrigger = { 65.f,1.f,80.f };
	m_vecDoor[1]->setTransform(vScale, vRotate, vPos);
	m_vecDoor[1]->setTrigger(vTrigger);

	vScale = { 1.f,7.f,10.f };
	vPos = { 70.f, 3.5f,25.f };
	vTrigger = { 68.f,1.f,17.f };
	m_vecDoor[2]->setTransform(vScale, vRotate, vPos);
	m_vecDoor[2]->setTrigger(vTrigger);
}

void C1Stage::setClearBox()
{
	vector<CGameObject*>* pGameObjectList = getGameObjects(LAYERID::ENVIRONMENT, GAMEOBJECTID::CUBE);

	vector<CGameObject*>::iterator iter = pGameObjectList->begin();

	iter += 23;
	for (_int i = 0; i < 9; i++)
	{
		(*iter)->setActive(false);
		(*iter)->AddRef();
		m_vecClearBox.emplace_back(*iter);
		iter++;
	}
}

void C1Stage::FloorClear()
{
	m_bFloorClear = false;
	for (auto pClearBox : m_vecClearBox)
		pClearBox->setActive(true);
	m_pPlayer->setJumpCount(30);
}

C1Stage* C1Stage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	C1Stage* pInstance = new C1Stage(pDevice);
	if (FAILED(pInstance->Init_Scene()))
		Safe_Release(pInstance);
	return pInstance;
}

void C1Stage::Free()
{
	ClearWall();
	ClearCollision();
	CRenderer::GetInstance()->Clear_RenderList();
	Safe_Release(m_pLoading);
	CScene::Free();

	Safe_Release(m_pPlayer);

	for_each(m_vecDoorObserver.begin(), m_vecDoorObserver.end(), DeleteObj);
	m_vecDoorObserver.clear();
	m_vecDoorObserver.shrink_to_fit();

	for_each(m_vecDoor.begin(), m_vecDoor.end(), DeleteObj);
	m_vecDoor.clear();
	m_vecDoor.shrink_to_fit();

	for_each(m_vecClearBox.begin(), m_vecClearBox.end(), DeleteObj);
	m_vecClearBox.clear();
	m_vecClearBox.shrink_to_fit();
}
