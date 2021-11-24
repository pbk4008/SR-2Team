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
#include "BossStage.h"

C3Stage::C3Stage() : m_pLoading(nullptr), m_pPlayer(nullptr), m_b1(false), m_b10(false), m_b11(false)
, m_b3(false), m_b4(false), m_b5(false), m_b6(false), m_b7(false), m_b8(false), m_b9(false), m_b12(false), m_b2(false),
m_pPotal(nullptr), m_bPotalSpawn(false), m_bMeleeSpawn(false), m_bShootSpawn(false), m_bFlySpawn(false)
{

}

C3Stage::C3Stage(LPDIRECT3DDEVICE9 pDevice) : CScene(pDevice), m_pLoading(nullptr), m_pPlayer(nullptr), m_b1(false)
, m_b10(false), m_b11(false), m_b3(false), m_b4(false), m_b5(false), m_b6(false), m_b7(false), 
m_b8(false), m_b9(false), m_b12(false), m_b2(false), m_pPotal(nullptr), m_bPotalSpawn(false),
m_bMeleeSpawn(false), m_bShootSpawn(false), m_bFlySpawn(false)
{
	
}

C3Stage::~C3Stage()
{
}

HRESULT C3Stage::Init_Scene()
{
	FAILED_CHECK_RETURN(Init_Layer(), E_FAIL);

	m_pLoading = CLoading::Create(m_pDevice, SCENEID::BOSS_STAGE);

	Init_Fog(D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.f), D3DFOG_EXP2, TRUE, 0.075f);


	return S_OK;
}

_int C3Stage::Update_Scene(const _float& fDeltaTime)
{
	
	_int iExit = 0;
	iExit = CScene::Update_Scene(fDeltaTime);

	if (!m_bMeleeSpawn)
		LoadMeleeMon();
	if (!m_bShootSpawn)
		LoadShootMon();
	if (!m_bFlySpawn)
		LoadFlyMon();

	if (m_pPotal->getClear())
	{
		if (m_pLoading->getFinish())
		{
			CScene* pScene = nullptr;
			pScene = CBossStage::Create(m_pDevice);

			pScene->setLayer(LAYERID::LOADING, m_mapLayer[LAYERID::LOADING]);
			NULL_CHECK_RETURN(pScene, E_FAIL);

			FAILED_CHECK_RETURN(Change_Scene(pScene), E_FAIL);

			return iExit;
		}
	}

	m_bMeleeSpawn = true;
	m_bShootSpawn = true;
	m_bFlySpawn = true;
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

	m_pPotal = Clone_ObjProto<CPotal>(GAMEOBJECTID::POTAL);
	_vec3 vPos(10.f, 1.f, 10.f);
	_vec3 vRot( 0.f, 0.f, 0.f);
	_vec3 vScale(1.f,1.f,1.f);
	m_pPotal->setTransform(vScale, vRot, vPos);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::POTAL, m_pPotal));

	m_pPlayer->setModel(pModel);
	m_pPlayer->setCamera(pCam);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::PLAYER, pGameObject), E_FAIL);
	m_pPlayer->AddRef();

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

void C3Stage::Init_Fog(_ulong Color, _ulong Mode, BOOL UseRange, _float Density)
{
	float Start = 0.5f;
	float End = 50.f;

	m_pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	m_pDevice->SetRenderState(D3DRS_FOGCOLOR, Color);

	if (D3DFOG_LINEAR == Mode)
	{
		m_pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
		m_pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&Start));
		m_pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&End));
	}
	else
	{
		m_pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
		m_pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&Density));
	}

	// Enable range-based fog if desired (only supported for
	//   vertex fog).  For this example, it is assumed that UseRange
	//   is set to a nonzero value only if the driver exposes the 
	//   D3DPRASTERCAPS_FOGRANGE capability.
	// Note: This is slightly more performance intensive
	//   than non-range-based fog.
	if (UseRange)
		m_pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);
}

void C3Stage::LoadMeleeMon()
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

	pMeleeMon = nullptr;
	pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
	vPos = { 49.f,1.f,34.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pMeleeMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);

	pMeleeMon = nullptr;
	pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
	vPos = { 54.f,1.f,34.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pMeleeMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);

	pMeleeMon = nullptr;
	pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
	vPos = { 31.f,1.f,64.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pMeleeMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);

	/*pMeleeMon = nullptr;
	pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
	vPos = { 62.f,22.5f,65.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pMeleeMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);

	pMeleeMon = nullptr;
	pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
	vPos = { 20.f,22.5f,49.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pMeleeMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);

	pMeleeMon = nullptr;
	pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);
	vPos = { 15.f,22.5f,20.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pMeleeMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER1, pMeleeMon);*/
}

void C3Stage::LoadShootMon()
{
	CShootMon* pShootMon = nullptr;
	pShootMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
	_vec3 vPos = { 21.f,22.5f,31.f };
	_vec3 vScale = { 1.f,1.f,1.f };
	_vec3 vAngle = { 0.f,0.f,0.f };
	pShootMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2, pShootMon);

	pShootMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
	vPos = { 86.f,22.5f,47.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pShootMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2, pShootMon);

	pShootMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
	vPos = { 4.f,1.f,93.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pShootMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2, pShootMon);

	pShootMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
	vPos = { 10.f,1.f,93.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pShootMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2, pShootMon);

	pShootMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
	vPos = { 49.f,1.f,18.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pShootMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2, pShootMon);

	pShootMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
	vPos = { 54.f,1.f,19.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pShootMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2, pShootMon);

	pShootMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
	vPos = { 35.f,1.f,31.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pShootMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2, pShootMon);

	pShootMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
	vPos = { 66.f,1.f,51.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pShootMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2, pShootMon);

	pShootMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
	vPos = { 81.f,1.f,24.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pShootMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2, pShootMon);

	pShootMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
	vPos = { 40.f,22.5f,54.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pShootMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2, pShootMon);
		
	pShootMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
	vPos = { 63.f,22.5f,46.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pShootMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2, pShootMon);
		
	/*pShootMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
	vPos = { 17.f,22.5f,48.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pShootMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2, pShootMon);

	pShootMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);
	vPos = { 29.f,22.5f,29.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pShootMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER2, pShootMon);		*/
}

void C3Stage::LoadFlyMon()
{
	//11
	CFlyMon* pFlyMon = nullptr;
	pFlyMon = Clone_ObjProto<CFlyMon>(GAMEOBJECTID::MONSTER3);
	_vec3 vPos = { 51.f,22.5f,46.f };
	_vec3 vScale = { 1.f,1.f,1.f };
	_vec3 vAngle = { 0.f,0.f,0.f };
	pFlyMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER3, pFlyMon);

	pFlyMon = nullptr;
	pFlyMon = Clone_ObjProto<CFlyMon>(GAMEOBJECTID::MONSTER3);
	vPos = { 23.f,1.f,39.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pFlyMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER3, pFlyMon);

	pFlyMon = nullptr;
	pFlyMon = Clone_ObjProto<CFlyMon>(GAMEOBJECTID::MONSTER3);
	vPos = { 19.f,1.f,40.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pFlyMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER3, pFlyMon);

	pFlyMon = nullptr;
	pFlyMon = Clone_ObjProto<CFlyMon>(GAMEOBJECTID::MONSTER3);
	vPos = { 60.f,22.5f,27.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pFlyMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER3, pFlyMon);

	/*pFlyMon = nullptr;
	pFlyMon = Clone_ObjProto<CFlyMon>(GAMEOBJECTID::MONSTER3);
	vPos = { 45.f,22.5f,24.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pFlyMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER3, pFlyMon);

	pFlyMon = nullptr;
	pFlyMon = Clone_ObjProto<CFlyMon>(GAMEOBJECTID::MONSTER3);
	vPos = { 75.f,22.5f,65.f };
	vScale = { 1.f,1.f,1.f };
	vAngle = { 0.f,0.f,0.f };
	pFlyMon->LoadTransform(vScale, vAngle, vPos);
	Add_GameObject(LAYERID::GAME_LOGIC, GAMEOBJECTID::MONSTER3, pFlyMon);	*/
}