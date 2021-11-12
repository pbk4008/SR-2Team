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
#include "ShootMon.h"
#include "FlyMon.h"
#include "Shuriken.h"
#include "Bomb.h"

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
	//ToDo:������� �ҷ����� ��
	m_pTextureMgr = Init_TextureMgr();
	NULL_CHECK_RETURN(m_pTextureMgr, -1);

	Load_Terrain(L"../Bin/Resource/Data/Terrain2.Terraindat", L"Terrain1");
	//Texture�ҷ�����
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/MeleeMon/Idle/IDLE_000.png", L"MeleeMon_Idle", 1);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/MeleeMon/Walk/WALKF_00%d.png", L"MeleeMon_WalkF", 4);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/MeleeMon/Attack/ATTACK_00%d.png", L"MeleeMon_Attack", 3);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/MeleeMon/Death/DEATH_00%d.png", L"MeleeMon_Death", 9);

	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/ShootMon/Idle/IDLE_000.png", L"ShootMon_Idle", 1);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/ShootMon/Walk/WALKF_00%d.png", L"ShootMon_WalkF", 4);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/ShootMon/Attack/ATTACK_00%d.png", L"ShootMon_Attack", 2);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/ShootMon/Death/DEATH_00%d.png", L"ShootMon_Death", 8);

	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FlyMon/Idle/IDLE_000.png", L"FlyMon_Idle", 1);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FlyMon/Walk/MOVE_00%d.png", L"FlyMon_WalkF", 4);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FlyMon/Attack/ATTACK_00%d.png", L"FlyMon_Attack", 3);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FlyMon/Death/DEATH_00%d.png", L"FlyMon_Death", 7);

	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Attack/Sword/Player_Attack_Sword00%d.png", L"PlayerSwordAttack", 4);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Attack/Shuriken/Player_Attack_Shuriken00%d.png", L"PlayerShurikenAttack",8);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Attack/Bomb/Player_Attack_Bomb00%d.png", L"PlayerBombAttack",3);

	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Shuriken/Shuriken00%d.png", L"Shuriken", 4);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Bomb/Bomb00%d.png", L"Bomb", 2);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Bomb/BombSmoge00%d.png", L"BombSmoge", 4);

	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Test/monster.png", L"Monster", 1);
	
	//Component���� ����
	CComponent* pCom = nullptr;

	pCom = CAnimator::Create(m_pDevice);
	NULL_CHECK_RETURN(pCom, -1);
	Init_ComProto(COMPONENTID::ANIMATOR, pCom);

	_vec3 vEye = _vec3(0.f, 0.f, -10.f);
	_vec3 vAt = _vec3(0.f, 0.f, 1.f);
	_vec3 vUp = _vec3(0.f, 1.f, 0.f);

	pCom = CCamera::Create(m_pDevice, vEye, vAt, vUp, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pCom, -1);
	Init_ComProto(COMPONENTID::CAMERA, pCom);

	pCom = CCollision::Create(m_pDevice);
	NULL_CHECK_RETURN(pCom, -1);
	Init_ComProto(COMPONENTID::COLLISION, pCom);

	//////////////////////////////////////////////////////////////////////////////////
	CGameObject* pObj = nullptr;
	//Player
	pObj = CPlayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::PLAYER, pObj);

	////Camera
	pObj = CMainCamera::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::CAMERA, pObj);

	//PlayerModel
	pObj = CPlayerModel::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::PLAYERMODEL, pObj);
	
	// Melee Monster #1
	pObj = CMeleeMon::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::MONSTER1, pObj);
	
	// Shoot Monster
	pObj = CShootMon::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::MONSTER2, pObj);
	
	// Fly Monster
	pObj = CFlyMon::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::MONSTER3, pObj);

	pObj = CTerrain::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::TERRAIN, pObj);

	pObj = CShuriken::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::SHURIKEN, pObj);

	pObj = CBomb::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::BOMB, pObj);
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
		MSG_BOX(L"Terrain Load : CreateFile : ����");
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
