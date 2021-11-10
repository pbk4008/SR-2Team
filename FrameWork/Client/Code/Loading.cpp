#include "pch.h"
#include "Loading.h"
#include "BackGround.h"
#include "MeleeMon.h"
#include "Player.h"
#include "Terrain.h"
#include "MainCamera.h"
#include "PlayerModel.h"
#include "Animator.h"
#include "Player_AttackAnim.h"
#include "Player_IdleAnim.h"
#include "MeleeMon_Idle.h"
#include "Player_Walk.h"
#include "Collision.h"
#include "MeleeMon_WalkF.h"
#include "MeleeMon_Attack.h"

CLoading::CLoading() : m_eSceneID(SCENEID::STAGE_END), m_pDevice(nullptr), m_bFinish(false), m_pTextureMgr(nullptr)
{
	ZeroMemory(m_szLoading, sizeof(_tchar) * 256);
}

CLoading::CLoading(LPDIRECT3DDEVICE9 pDevice) : m_eSceneID(SCENEID::STAGE_END), m_pDevice(pDevice), m_bFinish(false), m_pTextureMgr(nullptr)
{
	m_pDevice->AddRef();
	ZeroMemory(m_szLoading, sizeof(_tchar) * 256);
}

CLoading::~CLoading()
{
}

HRESULT CLoading::Init_Loading(SCENEID eLoading)
{
	m_eSceneID = eLoading;
	m_pTextureMgr = Init_TextureMgr();
	m_pTextureMgr->AddRef();
	NULL_CHECK_RETURN(m_pTextureMgr, E_FAIL);

	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	return S_OK;
}

_uint CLoading::Loading_ForStage()
{
	//ToDo:쓰레드로 불러들일 곳
	m_pTextureMgr = Init_TextureMgr();
	NULL_CHECK_RETURN(m_pTextureMgr,99);

	Load_Terrain(L"../Bin/Resource/Data/Terrain2.Terraindat", L"Terrain1");
	//Texture불러오기
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Attack/Player_Attack%d.png", L"Player", 4);

	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/MeleeMon/Idle/IDLE_000.png", L"MeleeMon_Idle", 1);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/MeleeMon/Walk/WALKF_00%d.png", L"MeleeMon_WalkF", 4);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/MeleeMon/Attack/ATTACK_00%d.png", L"MeleeMon_Attack", 3);

	//Component원본 생성
	CComponent* pCom = nullptr;

	pCom = CTexture::Create(m_pDevice, m_pTextureMgr->getTexture(L"Terrain1", TEXTURETYPE::TEX_NORMAL));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	Init_ComProto(COMPONENTID::TERRAIN_TEX1,pCom);

	pCom = CTexture::Create(m_pDevice, m_pTextureMgr->getTexture(L"Player", TEXTURETYPE::TEX_NORMAL));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	Init_ComProto(COMPONENTID::PLAYER_TEX, pCom);

	pCom = CPlayer_AttackAnim::Create(m_pDevice, Clone_ComProto<CTexture>(COMPONENTID::PLAYER_TEX));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	Init_ComProto(COMPONENTID::PLAYER_ATTACKANIM, pCom);
	
	pCom = CPlayerIdleAnim::Create(m_pDevice, Clone_ComProto<CTexture>(COMPONENTID::PLAYER_TEX));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	Init_ComProto(COMPONENTID::PLAYER_IDLEANIM, pCom);

	pCom = CPlayerWalk::Create(m_pDevice, Clone_ComProto<CTexture>(COMPONENTID::PLAYER_TEX));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	Init_ComProto(COMPONENTID::PLAYER_WALKANIM, pCom);

	// monster anim////////////////////////////////////////////////

	pCom = CTexture::Create(m_pDevice, m_pTextureMgr->getTexture(L"MeleeMon_Idle", TEXTURETYPE::TEX_NORMAL));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	Init_ComProto(COMPONENTID::MELEEMON_IDLETEX, pCom);

	pCom = CMeleeMon_Idle::Create(m_pDevice, Clone_ComProto<CTexture>(COMPONENTID::MELEEMON_IDLETEX));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	Init_ComProto(COMPONENTID::MELEEMON_IDLEANIM, pCom);

	pCom = CTexture::Create(m_pDevice, m_pTextureMgr->getTexture(L"MeleeMon_WalkF", TEXTURETYPE::TEX_NORMAL));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	Init_ComProto(COMPONENTID::MELEEMON_WALKTEX, pCom);

	pCom = CMeleeMon_WalkF::Create(m_pDevice, Clone_ComProto<CTexture>(COMPONENTID::MELEEMON_WALKTEX));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	Init_ComProto(COMPONENTID::MELEEMON_WALKANIM, pCom);

	pCom = CTexture::Create(m_pDevice, m_pTextureMgr->getTexture(L"MeleeMon_Attack", TEXTURETYPE::TEX_NORMAL));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	Init_ComProto(COMPONENTID::MELEEMON_ATTACKTEX, pCom);

	pCom = CMeleeMon_Attack::Create(m_pDevice, Clone_ComProto<CTexture>(COMPONENTID::MELEEMON_ATTACKTEX));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	Init_ComProto(COMPONENTID::MELEEMON_ATTACKANIM, pCom);

	//여기까지 ////////////////////////////////////////////////

	pCom = CAnimator::Create(m_pDevice);
	NULL_CHECK_RETURN(pCom, E_FAIL);
	Init_ComProto(COMPONENTID::ANIMATOR, pCom);

	_vec3 vEye = _vec3(0.f, 0.f, -10.f);
	_vec3 vAt = _vec3(0.f, 0.f, 1.f);
	_vec3 vUp = _vec3(0.f, 1.f, 0.f);

	pCom = CCamera::Create(m_pDevice, vEye, vAt, vUp, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pCom, E_FAIL);
	Init_ComProto(COMPONENTID::CAMERA, pCom);

	pCom = CCollision::Create(m_pDevice);
	NULL_CHECK_RETURN(pCom, E_FAIL);
	Init_ComProto(COMPONENTID::COLLISION, pCom);

	//////////////////////////////////////////////////////////////////////////////////
	CGameObject* pObj = nullptr;
	//Player
	pObj = CPlayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, E_FAIL);
	Init_ObjProto(GAMEOBJECTID::PLAYER, pObj);

	////Camera
	pObj = CMainCamera::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, E_FAIL);
	Init_ObjProto(GAMEOBJECTID::CAMERA, pObj);

	//PlayerModel
	pObj = CPlayerModel::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, E_FAIL);
	Init_ObjProto(GAMEOBJECTID::PLAYERMODEL, pObj);
	
	// Melee Monster #1
	pObj = CMeleeMon::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, E_FAIL);
	Init_ObjProto(GAMEOBJECTID::MONSTER, pObj);
	
	pObj = CTerrain::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, E_FAIL);
	Init_ObjProto(GAMEOBJECTID::TERRAIN, pObj);

	m_bFinish = true;
	return 0;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pDevice, SCENEID eID)
{
	CLoading* pInstance = new CLoading(pDevice);
	if(FAILED(pInstance->Init_Loading(eID)))
		Safe_Release(pInstance);
	return pInstance;
}

unsigned CLoading::Thread_Main(void* pArg)
{
	CLoading* pLoading = (CLoading*)pArg;

	_uint iFlag = 0;

	EnterCriticalSection(pLoading->getCrt());

	switch (pLoading->getLoadingID())
	{
	case SCENEID::STAGE_ONE:
		iFlag = pLoading->Loading_ForStage();
		break;
	case SCENEID::STAGE_TWO:
		break;
	}
	LeaveCriticalSection(pLoading->getCrt());
	return iFlag;
}

void CLoading::Free()
{
 	Safe_Release(m_pDevice);
	m_pTextureMgr->DestroyInstance();
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);
}

HRESULT CLoading::Load_Terrain(const _tchar* strPath, const _tchar* TerrainName)
{
	HANDLE hFile = CreateFile(strPath,
		GENERIC_READ,
		0,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MSG_BOX(L"Terrain Load : CreateFile : 실패");
		return E_FAIL;
	}

	DWORD dwByte = 0;

	_ulong dwTerrainX = 0;
	_ulong dwTerrainZ = 0;
	_ulong dwInterval = 0;
	_ulong dwTerrainDetail = 0;

	ReadFile(hFile, &dwTerrainX, sizeof(int), &dwByte, nullptr);
	ReadFile(hFile, &dwTerrainZ, sizeof(int), &dwByte, nullptr);
	ReadFile(hFile, &dwInterval, sizeof(int), &dwByte, nullptr);
	ReadFile(hFile, &dwTerrainDetail, sizeof(int), &dwByte, nullptr);

	int TerrainTextureFolderLength = 0;

	ReadFile(hFile, &TerrainTextureFolderLength, sizeof(int), &dwByte, nullptr);
	TCHAR* strFolder = new TCHAR[TerrainTextureFolderLength];
	ReadFile(hFile, strFolder, sizeof(TCHAR) * TerrainTextureFolderLength, &dwByte, nullptr);

	int TerrainTexturePathLength = 0;
	ReadFile(hFile, &TerrainTexturePathLength, sizeof(int), &dwByte, nullptr);
	TCHAR* strFile = new TCHAR[TerrainTexturePathLength];
	ReadFile(hFile, strFile, sizeof(TCHAR) * TerrainTexturePathLength, &dwByte, nullptr);

	TCHAR strtemp[MAX_PATH] = L"..\\..\\Client\\Bin\\Resource\\Texture\\";
	lstrcat(strtemp, strFolder);
	lstrcat(strtemp, L"\\");
	lstrcat(strtemp, strFile);
	lstrcat(strtemp, L".png");

	Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, strtemp, TerrainName, 1);

	delete[] strFolder;
	delete[] strFile;

	CloseHandle(hFile);

	CTerrainTex* pCom = CTerrainTex::Create(m_pDevice, dwTerrainX, dwTerrainZ, dwTerrainDetail, dwInterval);
	NULL_CHECK_RETURN(pCom, E_FAIL);
	Init_ComProto(COMPONENTID::TERRAINTEX, pCom);

	return S_OK;
}
