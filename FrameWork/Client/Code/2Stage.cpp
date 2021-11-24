#include "pch.h"
#include "2Stage.h"
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
#include "Boss.h"
#include "UI.h"
#include "Door.h"
#include "Potal.h"
#include "3Stage.h"


C2Stage::C2Stage() : m_pLoading(nullptr), m_pPlayer(nullptr)
{
}

C2Stage::C2Stage(LPDIRECT3DDEVICE9 pDevice) : CScene(pDevice), m_pLoading(nullptr)
, m_pPlayer(nullptr)

{
}

C2Stage::~C2Stage()
{
}

HRESULT C2Stage::Init_Scene()
{
	FAILED_CHECK_RETURN(Init_Layer(), E_FAIL);

	Init_Fog(D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.f), D3DFOG_EXP2, TRUE, 0.075f);

	//m_pLoading = CLoading::Create(m_pDevice, SCENEID::STAGE_TWO);
	
	//NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

_int C2Stage::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CScene::Update_Scene(fDeltaTime);

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

	if (m_pPotal->getClear())
	{
		if (m_pLoading->getFinish())
		{
			CScene* pScene = nullptr;
			pScene = C3Stage::Create(m_pDevice);

			pScene->setLayer(LAYERID::LOADING, m_mapLayer[LAYERID::LOADING]);
			NULL_CHECK_RETURN(pScene, E_FAIL);

			FAILED_CHECK_RETURN(Change_Scene(pScene), E_FAIL);

			return iExit;
		}
	}
	return iExit;
}

void C2Stage::LateUpdate_Scene()
{
	CScene::LateUpdate_Scene();
}

void C2Stage::Render_Scene()
{
}

HRESULT C2Stage::Init_Layer()
{
	FAILED_CHECK_RETURN(Init_Environment_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Init_GameLogic_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Init_UI_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Init_Loading_Layer(), E_FAIL);

	return S_OK;
}

HRESULT C2Stage::Init_Environment_Layer()
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	//TODO : Environment게임오브젝트 추가

	m_mapLayer.emplace(LAYERID::ENVIRONMENT, pLayer);

	return S_OK;
}

HRESULT C2Stage::Init_GameLogic_Layer()
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
	_vec3 vPos(48.f, 31.f, 61.5f);
	_vec3 vRot(0.f, 0.f, 0.f);
	_vec3 vScale(1.f, 1.f, 1.f);
	m_pPotal->setTransform(vScale, vRot, vPos);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::POTAL, m_pPotal));

	m_pPlayer->setModel(pModel);
	m_pPlayer->setCamera(pCam);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::PLAYER, pGameObject), E_FAIL);
	m_pPlayer->AddRef();

	m_mapLayer.emplace(LAYERID::GAME_LOGIC, pLayer);
	return S_OK;
}

HRESULT C2Stage::Init_UI_Layer()
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

HRESULT C2Stage::Init_Loading_Layer()
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	CGameObject* pGameObject = nullptr;

	m_mapLayer.emplace(LAYERID::LOADING, pLayer);
	return S_OK;
}

C2Stage* C2Stage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	C2Stage* pInstance = new C2Stage(pDevice);
	if (FAILED(pInstance->Init_Scene()))
		Safe_Release(pInstance);
	return pInstance;
}

void C2Stage::Init_Fog(_ulong Color, _ulong Mode, BOOL UseRange, _float Density)
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

void C2Stage::Free()
{
	ClearWall();
	ClearCollision();
	CRenderer::GetInstance()->Clear_RenderList();
	Safe_Release(m_pLoading);
	CScene::Free();

	Safe_Release(m_pPlayer);

}
