#include "pch.h"
#include "3Stage.h"
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
#include "UI.h"
#include "Door.h"
#include "Potal.h"

C3Stage::C3Stage() : m_pLoading(nullptr), m_pPlayer(nullptr), m_b1(false), m_b10(false), m_b11(false)
, m_b3(false), m_b4(false), m_b5(false), m_b6(false), m_b7(false), m_b8(false), m_b9(false), m_b12(false), m_b2(false)
{
	
}

C3Stage::C3Stage(LPDIRECT3DDEVICE9 pDevice) : CScene(pDevice), m_pLoading(nullptr), m_pPlayer(nullptr), m_b1(false)
, m_b10(false), m_b11(false), m_b3(false), m_b4(false), m_b5(false), m_b6(false), m_b7(false), 
m_b8(false), m_b9(false), m_b12(false), m_b2(false)

{
	
}

C3Stage::~C3Stage()
{
}

HRESULT C3Stage::Init_Scene()
{
	FAILED_CHECK_RETURN(Init_Layer(), E_FAIL);


	return S_OK;
}

_int C3Stage::Update_Scene(const _float& fDeltaTime)
{
	
	_int iExit = 0;
	iExit = CScene::Update_Scene(fDeltaTime);

	if (!m_b1)
	{
		//1
		CMeleeMon* pMeleeMon = nullptr;
		pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
		_vec3 vPos = { 7.f,1.f,48.f };
		_vec3 vScale = { 1.f,1.f,1.f };
		_vec3 vAngle = { 0.f,0.f,0.f };
		pMeleeMon->LoadTransform(vScale, vAngle, vPos);
		Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);

		//2
		pMeleeMon = nullptr;
		pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
		vPos = { 22.f,1.f,77.f };
		vScale = { 1.f,1.f,1.f };
		vAngle = { 0.f,0.f,0.f };
		pMeleeMon->LoadTransform(vScale, vAngle, vPos);
		Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);
	}

		/*
		pMeleeMon = nullptr;
		pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
		vPos = { 38.f,1.f,64.f };
		vScale = { 1.f,1.f,1.f };
		vAngle = { 0.f,0.f,0.f };
		pMeleeMon->LoadTransform(vScale, vAngle, vPos);
		Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);
	if (!m_b2)
	{//3
		CMeleeMon* pMeleeMon = nullptr;
		pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
		_vec3 vPos = { 22.f,1.f,10.f };
		_vec3 vScale = { 1.f,1.f,1.f };
		_vec3 vAngle = { 0.f,0.f,0.f };
		pMeleeMon->LoadTransform(vScale, vAngle, vPos);
		Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);
	}

		//4
		pMeleeMon = nullptr;
		pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
		vPos = { 38.f,1.f,64.f };
		vScale = { 1.f,1.f,1.f };
		vAngle = { 0.f,0.f,0.f };
		pMeleeMon->LoadTransform(vScale, vAngle, vPos);
		Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);

		//5
		pMeleeMon = nullptr;
		pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
		vPos = { 68.f,1.f,35.f };
		vScale = { 1.f,1.f,1.f };
		vAngle = { 0.f,0.f,0.f };
		pMeleeMon->LoadTransform(vScale, vAngle, vPos);
		Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);
		
		//6
		pMeleeMon = nullptr;
		pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
		vPos = { 93.f,1.f,71.f };
		vScale = { 1.f,1.f,1.f };
		vAngle = { 0.f,0.f,0.f };
		pMeleeMon->LoadTransform(vScale, vAngle, vPos);
		Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);

		//7
		pMeleeMon = nullptr;
		pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
		vPos = { 69.f,1.f, 4.f };
		vScale = { 1.f,1.f,1.f };
		vAngle = { 0.f,0.f,0.f };
		pMeleeMon->LoadTransform(vScale, vAngle, vPos);
		Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);

		//8
		pMeleeMon = nullptr;
		pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
		vPos = { 73.f,22.5f,25.f };
		vScale = { 1.f,1.f,1.f };
		vAngle = { 0.f,0.f,0.f };
		pMeleeMon->LoadTransform(vScale, vAngle, vPos);
		Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);

		//9
		CShootMon* pShootMon = nullptr;
		pShootMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
		vPos = { 21.f,22.5f,31.f };
		vScale = { 1.f,1.f,1.f };
		vAngle = { 0.f,0.f,0.f };
		pShootMon->LoadTransform(vScale, vAngle, vPos);
		Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2, pShootMon);

		//10
		pShootMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
		vPos = { 86.f,22.5f,47.f };
		vScale = { 1.f,1.f,1.f };
		vAngle = { 0.f,0.f,0.f };
		pShootMon->LoadTransform(vScale, vAngle, vPos);
		Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2, pShootMon);

		//11
		CFlyMon* pFlyMon = nullptr;
		pFlyMon = Clone_ObjProto<CFlyMon>(GAMEOBJECTID::MONSTER3);
		vPos = { 51.f,22.5f,46.f };
		vScale = { 1.f,1.f,1.f };
		vAngle = { 0.f,0.f,0.f };
		pFlyMon->LoadTransform(vScale, vAngle, vPos);
		Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER3, pFlyMon);

		//12
		pMeleeMon = nullptr;
		pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
		vPos = { 25.f,22.5f,53.f };
		vScale = { 1.f,1.f,1.f };
		vAngle = { 0.f,0.f,0.f };
		pMeleeMon->LoadTransform(vScale, vAngle, vPos);
		Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);

		*/

	m_b1 = true;
	//m_b2 = true;
	return iExit;
}

void C3Stage::LateUpdate_Scene()
{
	CScene::LateUpdate_Scene();
}

void C3Stage::Render_Scene()
{
}

HRESULT C3Stage::Init_Layer()
{
	FAILED_CHECK_RETURN(Init_Environment_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Init_GameLogic_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Init_UI_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Init_Loading_Layer(), E_FAIL);

	return S_OK;
}

HRESULT C3Stage::Init_Environment_Layer()
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	//TODO : Environment게임오브젝트 추가

	m_mapLayer.emplace(LAYERID::ENVIRONMENT, pLayer);

	return S_OK;
}

HRESULT C3Stage::Init_GameLogic_Layer()
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

	/*CMeleeMon* m_pMeleeMon = nullptr;
	pGameObject = m_pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::MONSTER1, pGameObject), E_FAIL);

	CShootMon* m_pShootMonn = nullptr;
	pGameObject = m_pShootMonn = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::MONSTER2, pGameObject), E_FAIL);

	CFlyMon* m_pFlyMonn = nullptr;
	pGameObject = m_pFlyMonn = Clone_ObjProto<CFlyMon>(GAMEOBJECTID::MONSTER3);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::MONSTER3, pGameObject), E_FAIL);*/

	/*CMeleeMon* m_pMeleeMon = nullptr;
	pGameObject = m_pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
	_vec3 vPos = { 40.f,1.f,35.f };
	m_pMeleeMon->getTransform()->setPos(vPos);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::MONSTER1, pGameObject), E_FAIL);*/



	m_mapLayer.emplace(LAYERID::GAME_LOGIC, pLayer);
	return S_OK;
}

HRESULT C3Stage::Init_UI_Layer()
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

HRESULT C3Stage::Init_Loading_Layer()
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	CGameObject* pGameObject = nullptr;

	m_mapLayer.emplace(LAYERID::LOADING, pLayer);
	return S_OK;
}

C3Stage* C3Stage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	C3Stage* pInstance = new C3Stage(pDevice);
	if (FAILED(pInstance->Init_Scene()))
		Safe_Release(pInstance);
	return pInstance;
}

void C3Stage::Free()
{
	ClearWall();
	ClearCollision();
	CRenderer::GetInstance()->Clear_RenderList();
	Safe_Release(m_pLoading);
	CScene::Free();

	Safe_Release(m_pPlayer);
}

/*
	CMeleeMon* pMeleeMon = nullptr;
	pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
	_vec3 vPos = { 22.f,1.f,77.f };
	_vec3 vScale = { 1.f,1.f,1.f };
	_vec3 vAngle = { 0.f,0.f,0.f };
	pMeleeMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);


	//3
	pMeleeMon = nullptr;
	pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
	vPos = { 22.f,1.f,10.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pMeleeMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);

	//4
	pMeleeMon = nullptr;
	pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
	vPos = { 38.f,1.f,64.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pMeleeMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);

	//5
	pMeleeMon = nullptr;
	pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
	vPos = { 68.f,1.f,35.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pMeleeMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);

	//6
	pMeleeMon = nullptr;
	pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
	vPos = { 93.f,1.f,71.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pMeleeMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);

	//7
	pMeleeMon = nullptr;
	pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
	vPos = { 69.f,1.f, 4.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pMeleeMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);

	//8
	pMeleeMon = nullptr;
	pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
	vPos = { 73.f,22.5f,25.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pMeleeMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);

	//9
	CShootMon* pShootMon = nullptr;
	pShootMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
	vPos = { 21.f,22.5f,31.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pShootMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2, pShootMon);

	//10
	pShootMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
	vPos = { 86.f,22.5f,47.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pShootMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2, pShootMon);

	//11
	CFlyMon* pFlyMon = nullptr;
	pFlyMon = Clone_ObjProto<CFlyMon>(GAMEOBJECTID::MONSTER3);
	vPos = { 51.f,22.5f,46.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pFlyMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER3, pFlyMon);

	//12
	pMeleeMon = nullptr;
	pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
	vPos = { 25.f,22.5f,53.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pMeleeMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);

	*/