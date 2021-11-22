#include "pch.h"
#include "Logo.h"
#include "Layer.h"
#include "Loading.h"
#include "BackGround.h"
#include "Transform.h"
#include "1Stage.h"
#include "BackGround.h"
#include "2Stage.h"
#include "3Stage.h"


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
	Init_LogoScene();
	FAILED_CHECK_RETURN(Init_Layer(), E_FAIL);
	FAILED_CHECK_RETURN(Init_ProtoMgr(), E_FAIL);

	//m_pLoading = CLoading::Create(m_pDevice, SCENEID::STAGE_ONE);
	m_pLoading = CLoading::Create(m_pDevice, SCENEID::STAGE_TWO);
	//m_pLoading = CLoading::Create(m_pDevice, SCENEID::STAGE_ONE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

_int CLogo::Update_Scene(const _float& fDeltaTime)
{
	_int iExit = 0;
	iExit = CScene::Update_Scene(fDeltaTime);
	if (m_pLoading->getFinish())
	{
		//ToDo:로딩 씬에서 다음씬으로 넘어가는 상호작용
		if (Key_Down(VIR_ENTER))
		{
			CScene* pScene = nullptr;

			pScene = C2Stage::Create(m_pDevice);

			pScene->setLayer(LAYERID::LOADING, m_mapLayer[LAYERID::LOADING]);
			NULL_CHECK_RETURN(pScene, E_FAIL);

			FAILED_CHECK_RETURN(Change_Scene(pScene), E_FAIL);

			return iExit;
		}
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
	FAILED_CHECK_RETURN(Init_LoadingLayer(), E_FAIL);

	return S_OK;
}

HRESULT CLogo::Init_Environment_Layer()
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	//TODO : Environment게임오브젝트 추가
	CGameObject* pGameObject = nullptr;
	pGameObject = Clone_ObjProto<CBackGround>(GAMEOBJECTID::BACKGROUND);
	FAILED_CHECK_RETURN(pLayer->Add_Object(GAMEOBJECTID::BACKGROUND, pGameObject), E_FAIL);
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

HRESULT CLogo::Init_LoadingLayer()
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	//TODO : UI 게임오브젝트 추가
	CGameObject* pGameObject = nullptr;

	m_mapLayer.emplace(LAYERID::LOADING, pLayer);
	return S_OK;
}

HRESULT CLogo::Init_LogoScene()
{
	//Logo씬에서 사용할 텍스쳐
	CTextureMgr* pTexutreMgr = Init_TextureMgr();
	pTexutreMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Etc/BackGround/BackGround.png", L"BackGround", 1);

	//Component 원본
	CComponent* pCom = nullptr;

	pCom = CTexture::Create(m_pDevice);
	NULL_CHECK_RETURN(pCom, -1);
	Init_ComProto(COMPONENTID::TEXTURE, pCom);

	pCom = CRcTex::Create(m_pDevice);
	NULL_CHECK_RETURN(pCom, E_FAIL);
	Init_ComProto(COMPONENTID::RCTEX, pCom);

	pCom = CTransform::Create();
	NULL_CHECK_RETURN(pCom, E_FAIL);
	Init_ComProto(COMPONENTID::TRANSFORM, pCom);

	//GameObject원본 생성
	CGameObject* pObj = nullptr;
	pObj = CBackGround::Create(m_pDevice, SCENEID::STAGE_TWO);
	NULL_CHECK_RETURN(pObj, E_FAIL);
	Init_ObjProto(GAMEOBJECTID::BACKGROUND, pObj);

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
	CRenderer::GetInstance()->Clear_RenderList();
	Safe_Release(m_pLoading);
	CScene::Free();
}
