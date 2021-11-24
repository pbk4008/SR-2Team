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
#include "SoundMgr.h"

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

	Init_Fog(D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.f), D3DFOG_EXP2, TRUE, 0.075f);

	CSoundMgr::Get_Instance()->PlayBGM((TCHAR*)L"BossStage.mp3");
	
	return S_OK;
}

_int CBossStage::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CScene::Update_Scene(fDeltaTime);

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

void CBossStage::Init_Fog(_ulong Color, _ulong Mode, BOOL UseRange, _float Density)
{
	float Start = 0.5f;
	float End = 50.f;

	m_pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	m_pDevice->SetRenderState(D3DRS_FOGCOLOR, Color);

	if (D3DFOG_LINEAR == Mode)
	{
		m_pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
		m_pDevice->SetRenderState(D3DRS_FOGSTART, (DWORD)(&Start));
		m_pDevice->SetRenderState(D3DRS_FOGEND, (DWORD)(&End));
	}
	else
	{
		m_pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
		m_pDevice->SetRenderState(D3DRS_FOGDENSITY, (DWORD)(&Density));
	}

	if (UseRange)
		m_pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);
}
