#include "pch.h"
#include "Loading.h"
#include "BackGround.h"
#include "MeleeMon.h"
#include "Player.h"
#include "Terrain.h"
#include "MainCamera.h"
#include "PlayerModel.h"
#include "Animator.h"
#include "ShootMon.h"
#include "FlyMon.h"
#include "Shuriken.h"
#include "Bomb.h"
#include "MonBullet.h"
#include "Cube.h"
#include "Quad.h"
#include "Fog.h"
#include "Boss.h"
#include "Fireball.h"
#include "Item.h"
#include "Ui.h"
#include "NaviMesh.h"
#include "HP.h"
#include "Door.h"
#include "Spawner.h"
#include "Key.h"

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
	m_pIniManager = INIManager::GetInstance();
	NULL_CHECK_RETURN(m_pTextureMgr, E_FAIL);

	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	return S_OK;
}

_uint CLoading::Loading_ForStage1()
{
	//ToDo:쓰레드로 불러들일 곳
	m_pTextureMgr = Init_TextureMgr();
	NULL_CHECK_RETURN(m_pTextureMgr, -1);
	//Texture불러오기
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/MeleeMon/Idle/IDLE_000.png", L"MeleeMon_Idle", 1);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/MeleeMon/Walk/WALKF_00%d.png", L"MeleeMon_WalkF", 4);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/MeleeMon/Attack/ATTACK_00%d.png", L"MeleeMon_Attack", 3);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/MeleeMon/Death/DEATH_00%d.png", L"MeleeMon_Death", 9);

	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/ShootMon/Idle/IDLE_000.png", L"ShootMon_Idle", 1);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/ShootMon/Walk/WALKF_00%d.png", L"ShootMon_WalkF", 4);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/ShootMon/Attack/ATTACK_00%d.png", L"ShootMon_Attack", 2);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/ShootMon/Death/DEATH_00%d.png", L"ShootMon_Death", 8);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/ShootMon/MonBullet/MONBULLET_000.png", L"MonBullet", 1);

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

	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Etc/Door/Door00%d.png", L"Door", 1);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Etc/Spawner/Spawner00%d.png", L"Spawner", 8);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Etc/Key/Key000.png", L"Key", 1);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Etc/Potal/Potal00%d.png", L"Potal", 15);

	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, HP20PATH	, L"HP20", 1);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, HP50PATH, L"HP50", 1);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, HP100PATH, L"HP100", 1);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, SHURIKEN20PATH, L"SHURIKEN20", 1);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, SHURIKEN50PATH, L"SHURIKEN50", 1);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, BOMB2PATH, L"BOMB2", 1);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, BOMB5PATH, L"BOMB5", 1);

	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/UI/UI.png", L"UI", 1);

	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Idle/IDLE_000.png", L"Boss_Idle", 1);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Fire1/FIRE_00%d.png", L"Boss_Fire1", 28);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Fire2/FIRE2_00%d.png", L"Boss_Fire2", 15);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Fire3/FIRE3_00%d.png", L"Boss_Fire3", 22);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Fireball/FIREBALL_00%d.png", L"Boss_Fireball", 4);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Charge/CHARGE_00%d.png", L"Boss_Charge", 3);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Death/DEATH_00%d.png", L"Boss_Death", 12);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/MeleeAttack/MELEE_00%d.png", L"Boss_Melee", 5);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/RangeAttack/RANGE1_00%d.png", L"Boss_Range", 4);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Move/MOVE_00%d.png", L"Boss_Move", 3);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/HP/HP_EMPTY.png", L"Boss_HPFull", 1);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/HP/HP_FULL.png", L"Boss_HPEmpty", 1);
	
	//Component원본 생성
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

	pCom = CSphereCollision::Create(m_pDevice);
	NULL_CHECK_RETURN(pCom, -1);
	Init_ComProto(COMPONENTID::SPHERECOL, pCom);

	pCom = CBoxCollision::Create(m_pDevice);
	NULL_CHECK_RETURN(pCom, -1);
	Init_ComProto(COMPONENTID::BOXCOL, pCom);

	pCom = CNaviMesh::Create(m_pDevice,0,0);
	NULL_CHECK_RETURN(pCom, -1);
	Init_ComProto(COMPONENTID::NAVIMESH, pCom);

	pCom = CCubeTex::Create(m_pDevice);
	NULL_CHECK_RETURN(pCom, -1);
	Init_ComProto(COMPONENTID::CUBETEX, pCom);

	//////////////////////////////////////////////////////////////////////////////////
	CGameObject* pObj = nullptr;

	//Quad
	pObj = CQuad::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::QUAD, pObj);

	//Cube
	pObj = CCube::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::CUBE, pObj);

	////Player
	pObj = CPlayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::PLAYER, pObj);

	////////Camera
	pObj = CMainCamera::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::CAMERA, pObj);

	////PlayerModel
	pObj = CPlayerModel::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::PLAYERMODEL, pObj);
	
	// Melee Monster #1
	pObj = CMeleeMon::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::MONSTER1, pObj);
	//
	//// Shoot Monster
	//pObj = CShootMon::Create(m_pDevice);
	//NULL_CHECK_RETURN(pObj, -1);
	//Init_ObjProto(GAMEOBJECTID::MONSTER2, pObj);

	//// MonBullet
	//pObj = CMonBullet::Create(m_pDevice);
	//NULL_CHECK_RETURN(pObj, -1);
	//Init_ObjProto(GAMEOBJECTID::MONBULLET, pObj);
	
	// Fly Monster
	//pObj = CFlyMon::Create(m_pDevice);
	//NULL_CHECK_RETURN(pObj, -1);
	//Init_ObjProto(GAMEOBJECTID::MONSTER3, pObj);

	//// MonBullet
	//pObj = CMonBullet::Create(m_pDevice);
	//NULL_CHECK_RETURN(pObj, -1);
	//Init_ObjProto(GAMEOBJECTID::MONBULLET, pObj);
	//
	//// Fly Monster
	//pObj = CFlyMon::Create(m_pDevice);
	//NULL_CHECK_RETURN(pObj, -1);
	//Init_ObjProto(GAMEOBJECTID::MONSTER3, pObj);

	//pObj = CBoss::Create(m_pDevice);
	//NULL_CHECK_RETURN(pObj, -1);
	//Init_ObjProto(GAMEOBJECTID::BOSS, pObj);

	pObj = CShuriken::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::SHURIKEN, pObj);

	pObj = CBomb::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::BOMB, pObj);

	pObj = CFog::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::FOG, pObj);

	pObj = CItem::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::ITEM, pObj);

	pObj = CUI::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::UI, pObj);
	
	pObj = CDoor::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::DOOR, pObj);

	pObj = CSpawner::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::SPAWNER, pObj);

	pObj = CKey::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::KEY, pObj);


	FAILED_CHECK_RETURN(Load_Quad(L"Stage2QuadData"),E_FAIL);
	FAILED_CHECK_RETURN(Load_Cube(L"Stage2CubeData"),E_FAIL);
	FAILED_CHECK_RETURN(Load_Item(L"Stage1ItemData"), E_FAIL);
	FAILED_CHECK_RETURN(Load_Terrain(L"Stage2TerrainData"),E_FAIL);

	m_bFinish = true;
	return 0;
}

_uint CLoading::Loading_ForStage2()
{
	m_pTextureMgr = Init_TextureMgr();
	NULL_CHECK_RETURN(m_pTextureMgr, -1);

	/*FAILED_CHECK_RETURN(Load_Quad(L"Stage2QuadData"), E_FAIL);
	FAILED_CHECK_RETURN(Load_Cube(L"Stage2CubeData"), E_FAIL);
	FAILED_CHECK_RETURN(Load_Item(L"Stage2ItemData"), E_FAIL);
	FAILED_CHECK_RETURN(Load_Terrain(L"Stage2TerrainData"), E_FAIL);*/

	m_bFinish = true;
	return 0;
}

_uint CLoading::Loading_ForStage3()
{
	m_pTextureMgr = Init_TextureMgr();
	NULL_CHECK_RETURN(m_pTextureMgr, -1);

	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Idle/IDLE_000.png", L"Boss_Idle", 1);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Fire1/FIRE_00%d.png", L"Boss_Fire1", 28);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Fire2/FIRE2_00%d.png", L"Boss_Fire2", 15);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Fire3/FIRE3_00%d.png", L"Boss_Fire3", 22);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Fireball/FIREBALL_00%d.png", L"Boss_Fireball", 4);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Charge/CHARGE_00%d.png", L"Boss_Charge", 3);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Death/DEATH_00%d.png", L"Boss_Death", 12);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/MeleeAttack/MELEE_00%d.png", L"Boss_Melee", 5);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/RangeAttack/RANGE1_00%d.png", L"Boss_Range", 4);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Move/MOVE_00%d.png", L"Boss_Move", 3);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/HP/HP_EMPTY.png", L"Boss_HPFull", 1);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Boss/HP/HP_FULL.png", L"Boss_HPEmpty", 1);

	//pObj = CHP::Create(m_pDevice);
	//NULL_CHECK_RETURN(pObj, -1);
	//Init_ObjProto(GAMEOBJECTID::BOSSHP, pObj);

	//pObj = CBoss::Create(m_pDevice);
	//NULL_CHECK_RETURN(pObj, -1);
	//Init_ObjProto(GAMEOBJECTID::BOSS, pObj);

	//pObj = CFireball::Create(m_pDevice);
	//NULL_CHECK_RETURN(pObj, -1);
	//Init_ObjProto(GAMEOBJECTID::FIREBALL, pObj);

	FAILED_CHECK_RETURN(Load_Quad(L"Stage3QuadData"), E_FAIL);
	FAILED_CHECK_RETURN(Load_Cube(L"Stage3CubeData"), E_FAIL);
	FAILED_CHECK_RETURN(Load_Item(L"Stage3ItemData"), E_FAIL);
	FAILED_CHECK_RETURN(Load_Terrain(L"Stage3TerrainData"), E_FAIL);

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
		iFlag = pLoading->Loading_ForStage1();
		break;
	case SCENEID::STAGE_TWO:
		iFlag = pLoading->Loading_ForStage2();
		break;
	case SCENEID::STAGE_THREE:
		iFlag = pLoading->Loading_ForStage3();
		break;
	}
	LeaveCriticalSection(pLoading->getCrt());
	return iFlag;
}

void CLoading::Free()
{
 	Safe_Release(m_pDevice);
	m_pIniManager->DestroyInstance();
	m_pTextureMgr->DestroyInstance();
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);
}

HRESULT CLoading::Load_Terrain(const _tchar* strName)
{
	wstring tmp = strName;
	string strFile(tmp.begin(), tmp.end());
	int TerrainSize = m_pIniManager->LoadDataInteger(strFile, "TerrainCount", "Count");
	
	string Section;
	string Key;
	string Value;
	CTerrain* pTerrain = nullptr;

	for (int i = 0; i < TerrainSize; ++i)
	{
		Section = string_format("Terrain_%d", i);
		Key = "Info";

		string VtxInfoValue = m_pIniManager->LoadDataString(strFile, Section, Key);

		vector<_int> iVecTerrainInfo;
		size_t dot = 0;
		int PointerSize = 0;
		while (true)
		{
			if (VtxInfoValue.find(',') == string::npos)
			{
				Value = VtxInfoValue.substr(0, VtxInfoValue.size());
				iVecTerrainInfo.emplace_back(stoi(Value));
				PointerSize = 0;
				break;
			}
			dot = VtxInfoValue.find(',');
			Value = VtxInfoValue.substr(0, dot);
			iVecTerrainInfo.emplace_back(stoi(Value));

			VtxInfoValue.erase(0, dot + 1);
		}
		//지형 생성
		if (i==0)
		{
			CTerrainTex* pTerrainTex = CTerrainTex::Create(m_pDevice, 2,2,1,1);
			NULL_CHECK_RETURN(pTerrainTex, E_FAIL);
			Init_ComProto(COMPONENTID::TERRAINTEX, pTerrainTex);

			CGameObject* pObj = CTerrain::Create(m_pDevice);
			NULL_CHECK_RETURN(pObj, E_FAIL);
			FAILED_CHECK_RETURN(Init_ObjProto(GAMEOBJECTID::TERRAIN, pObj) , E_FAIL);
		}
		
		pTerrain = Clone_ObjProto<CTerrain>(GAMEOBJECTID::TERRAIN);
		pTerrain->setVtxSetting(iVecTerrainInfo[0], iVecTerrainInfo[1], iVecTerrainInfo[2], iVecTerrainInfo[3]);
		//지형 텍스쳐 불러오기
		wstring FolderName;
		wstring FileName;

		Key = "FileFolderName";
		
		string ObjectAndTypeName = m_pIniManager->LoadDataString(strFile, Section, Key);

		while (true)
		{
			if (ObjectAndTypeName.find(',') == std::string::npos)
			{
				Value = ObjectAndTypeName.substr(0, ObjectAndTypeName.size());
				FileName.assign(Value.begin(), Value.end());
				break;
			}
			dot = ObjectAndTypeName.find(',');
			Value = ObjectAndTypeName.substr(0, dot);
			FolderName.assign(Value.begin(), Value.end());
			ObjectAndTypeName.erase(0, dot + 1);
		}

		TCHAR strtemp[MAX_PATH] = L"..\\..\\Client\\Bin\\Resource\\Texture\\";
		lstrcat(strtemp, FolderName.c_str());
		lstrcat(strtemp, L"\\");
		lstrcat(strtemp, FileName.c_str());
		lstrcat(strtemp, L".png");
		if (!GetTexture(FileName.c_str(), TEXTURETYPE::TEX_NORMAL))
			Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, strtemp, FileName.c_str(), 1);

		//Todo:지형 텍스쳐 넣기
		pTerrain->setTexture(FileName.c_str());

		//지형 Scale값
		Key = "Scale";
		
		string strScale = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 Scale{};

		while (true)
		{
			if (strScale.find(',') == string::npos)
			{
				Value = strScale.substr(0, strScale.size());
				*(((float*)&Scale) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strScale.find(',');
			Value = strScale.substr(0, dot);
			*(((float*)&Scale) + (PointerSize++)) = stof(Value);
			strScale.erase(0, dot + 1);
		}

		Key = "Euler Angle";
		
		string strAngle = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 Roatate{};
		while (true)
		{
			if (strAngle.find(',') == string::npos)
			{
				Value = strAngle.substr(0, strAngle.size());
				*(((float*)&Roatate) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strAngle.find(',');
			Value = strAngle.substr(0, dot);
			*(((float*)&Roatate) + (PointerSize++)) = stof(Value);
			strAngle.erase(0, dot + 1);
		}

		Key = "Position";
	
		string strPos = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 Position{};
		while (true)
		{
			if (strPos.find(',') == string::npos)
			{
				Value = strPos.substr(0, strPos.size());
				*(((float*)&Position) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strPos.find(',');
			Value = strPos.substr(0, dot);
			*(((float*)&Position) + (PointerSize++)) = stof(Value);
			strPos.erase(0, dot + 1);
		}

		pTerrain->LoadTransform(Scale, Roatate, Position);
		pTerrain->setActive(false);
		pTerrain->Create_NaviMesh();
		Add_GameObject(LAYERID::LOADING, GAMEOBJECTID::TERRAIN, pTerrain);
	}
	return S_OK;
}

HRESULT CLoading::Load_Quad(const _tchar* strName)
{
	wstring tmp = strName;
	string strFile(tmp.begin(), tmp.end());
	
	int QuadSize = m_pIniManager->LoadDataInteger(strFile, "QuadCount", "Count");

	string Section;
	string Key;
	string Value;
	CQuad* pQuad = nullptr;

	for (int i = 0; i < QuadSize; ++i)
	{
		Section = string_format("Quad_%d", i);
		size_t dot = 0;
		int PointerSize = 0;

		pQuad = Clone_ObjProto<CQuad>(GAMEOBJECTID::QUAD);

		wstring FolderName;
		wstring FileName;

		Key = "FolderFileName";
		
		string ObjectAndTypeName = m_pIniManager->LoadDataString(strFile, Section, Key);

		while (true)
		{
			if (ObjectAndTypeName.find(',') == string::npos)
			{
				Value = ObjectAndTypeName.substr(0, ObjectAndTypeName.size());
				FileName.assign(Value.begin(), Value.end());
				break;
			}
			dot = ObjectAndTypeName.find(',');
			Value = ObjectAndTypeName.substr(0, dot);
			FolderName.assign(Value.begin(), Value.end());
			ObjectAndTypeName.erase(0, dot + 1);
		}

		TCHAR strtemp[MAX_PATH] = L"..\\..\\Client\\Bin\\Resource\\Texture\\";
		lstrcat(strtemp, FolderName.c_str());
		lstrcat(strtemp, L"\\");
		lstrcat(strtemp, FileName.c_str());
		lstrcat(strtemp, L".png");
		if (!GetTexture(FileName.c_str(), TEXTURETYPE::TEX_NORMAL))
			Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, strtemp, FileName.c_str(), 1);

		//Quad텍스쳐 넣기
		pQuad->setTexture(FileName.c_str());

		Key = "Scale";
		
		string strScale = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 Scale{};

		while (true)
		{
			if (strScale.find(',') == string::npos)
			{
				Value = strScale.substr(0, strScale.size());
				*(((float*)&Scale) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strScale.find(',');
			Value = strScale.substr(0, dot);
			*(((float*)&Scale) + (PointerSize++)) = stof(Value);
			strScale.erase(0, dot + 1);
		}

		Key = "Euler Angle";
		
		string strAngle = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 vRotate{};
		while (true)
		{
			if (strAngle.find(',') == string::npos)
			{
				Value = strAngle.substr(0, strAngle.size());
				*(((float*)&vRotate) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strAngle.find(',');
			Value = strAngle.substr(0, dot);
			*(((float*)&vRotate) + (PointerSize++)) = stof(Value);
			strAngle.erase(0, dot + 1);
		}

		Key = "Position";
		
		string strPos = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 Position{};
		while (true)
		{
			if (strPos.find(',') == string::npos)
			{
				Value = strPos.substr(0, strPos.size());
				*(((float*)&Position) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strPos.find(',');
			Value = strPos.substr(0, dot);
			*(((float*)&Position) + (PointerSize++)) = stof(Value);
			strPos.erase(0, dot + 1);
		}

		//QuadTransform 설정
		pQuad->getTransform()->setScale(Scale);
		pQuad->getTransform()->setAngle(vRotate);
		pQuad->getTransform()->setPos(Position);
		pQuad->setActive(false);
		Add_GameObject(LAYERID::LOADING, GAMEOBJECTID::QUAD, pQuad);
	}
	return S_OK;
}

HRESULT CLoading::Load_Cube(const _tchar* strName)
{
	wstring tmp = strName;
	string strFile(tmp.begin(), tmp.end());
	

	int QuadSize = m_pIniManager->LoadDataInteger(strFile, "CubeCount", "Count");

	string Section;
	string Key;
	string Value;
	CCube* pCube = nullptr;

	for (int i = 0; i < QuadSize; ++i)
	{
		Section = string_format("Cube_%d", i);
		size_t dot = 0;
		int PointerSize = 0;

		pCube = Clone_ObjProto<CCube>(GAMEOBJECTID::CUBE);

		wstring FolderName;
		wstring FileName;

		Key = "FolderFileName";
		
		string ObjectAndTypeName = m_pIniManager->LoadDataString(strFile, Section, Key);
		while (true)
		{
			dot = ObjectAndTypeName.find(',');
			Value = ObjectAndTypeName.substr(0, dot);
			FolderName.assign(Value.begin(), Value.end());
			ObjectAndTypeName.erase(0, dot + 1);

			if (ObjectAndTypeName.find(',') == string::npos)
			{
				Value = ObjectAndTypeName.substr(0, ObjectAndTypeName.size());
				FileName.assign(Value.begin(), Value.end());

				TCHAR strtemp[MAX_PATH] = L"..\\..\\Client\\Bin\\Resource\\Texture\\";
				lstrcat(strtemp, FolderName.c_str());
				lstrcat(strtemp, L"\\");
				lstrcat(strtemp, FileName.c_str());
				lstrcat(strtemp, L".png");
				if (!GetTexture(FileName.c_str(), TEXTURETYPE::TEX_NORMAL))
					Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, strtemp, FileName.c_str(), 1);

				pCube->setTexture(FileName.c_str(), PointerSize);
				
				PointerSize = 0;
				break;
			}

			dot = ObjectAndTypeName.find(',');
			Value = ObjectAndTypeName.substr(0, dot);
			FileName.assign(Value.begin(), Value.end());
			ObjectAndTypeName.erase(0, dot + 1);

			TCHAR strtemp[MAX_PATH] = L"..\\..\\Client\\Bin\\Resource\\Texture\\";
			lstrcat(strtemp, FolderName.c_str());
			lstrcat(strtemp, L"\\");
			lstrcat(strtemp, FileName.c_str());
			lstrcat(strtemp, L".png");
			if (!GetTexture(FileName.c_str(), TEXTURETYPE::TEX_NORMAL))
				Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, strtemp, FileName.c_str(), 1);

			pCube->setTexture(FileName.c_str(), PointerSize);
			++PointerSize;
		}

		Key = "Scale";
		
		string strScale = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 Scale{};

		while (true)
		{
			if (strScale.find(',') == string::npos)
			{
				Value = strScale.substr(0, strScale.size());
				*(((float*)&Scale) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strScale.find(',');
			Value = strScale.substr(0, dot);
			*(((float*)&Scale) + (PointerSize++)) = stof(Value);
			strScale.erase(0, dot + 1);
		}

		Key = "Euler Angle";
	
		string strAngle = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 vRotate{};
		while (true)
		{
			if (strAngle.find(',') == string::npos)
			{
				Value = strAngle.substr(0, strAngle.size());
				*(((float*)&vRotate) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strAngle.find(',');
			Value = strAngle.substr(0, dot);
			*(((float*)&vRotate) + (PointerSize++)) = stof(Value);
			strAngle.erase(0, dot + 1);
		}

		Key = "Position";
		
		string strPos = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 Position{};
		while (true)
		{
			if (strPos.find(',') == string::npos)
			{
				Value = strPos.substr(0, strPos.size());
				*(((float*)&Position) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strPos.find(',');
			Value = strPos.substr(0, dot);
			*(((float*)&Position) + (PointerSize++)) = stof(Value);
			strPos.erase(0, dot + 1);
		}

		pCube->LoadTransform(Scale, vRotate, Position);
		pCube->setActive(false);
		Add_GameObject(LAYERID::LOADING, GAMEOBJECTID::CUBE, pCube);
	}
	return S_OK;
}

HRESULT CLoading::Load_Item(const _tchar* strName)
{
	wstring tmp = strName;
	string strFile(tmp.begin(), tmp.end());
	

	int ItemSize = m_pIniManager->LoadDataInteger(strFile, "ItemCount", "Count");

	string Section;
	string Key;
	string Value;
	CItem* pItem = nullptr;

	for (int i = 0; i < ItemSize; ++i)
	{
		Section = string_format("Item_%d", i);
		size_t dot = 0;
		int PointerSize = 0;

		pItem = Clone_ObjProto<CItem>(GAMEOBJECTID::ITEM);

		Key = "ObjectAndTypeName";

		std::string FileFolderName = m_pIniManager->LoadDataString(strFile, Section, Key);
		std::wstring ObjectName;
		std::wstring TypeName;
		while (true)
		{
			if (FileFolderName.find(',') == std::string::npos)
			{
				Value = FileFolderName.substr(0, FileFolderName.size());
				TypeName.assign(Value.begin(), Value.end());
				break;
			}
			dot = FileFolderName.find(',');
			Value = FileFolderName.substr(0, dot);
			ObjectName.assign(Value.begin(), Value.end());
			FileFolderName.erase(0, dot + 1);
		}
		
		if (!TypeName.compare(L"HP20"))
		{
			pItem->setItemPower(20);
		}
		else if (!TypeName.compare(L"HP50"))
		{
			pItem->setItemPower(50);
		}
		else if (!TypeName.compare(L"HP100"))
		{
			pItem->setItemPower(100);
		}
		else if (!TypeName.compare(L"SHURIKEN20"))
		{
			pItem->setItemPower(20);
		}
		else if (!TypeName.compare(L"SHURIKEN50"))
		{
			pItem->setItemPower(50);
		}
		else if (!TypeName.compare(L"BOMB2"))
		{
			pItem->setItemPower(2);
		}
		else if (!TypeName.compare(L"BOMB5"))
		{
			pItem->setItemPower(5);
		}
		
		pItem->setTexture(TypeName.c_str());
		
		Key = "Scale";
		string strScale = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 Scale{};

		while (true)
		{
			if (strScale.find(',') == string::npos)
			{
				Value = strScale.substr(0, strScale.size());
				*(((float*)&Scale) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strScale.find(',');
			Value = strScale.substr(0, dot);
			*(((float*)&Scale) + (PointerSize++)) = stof(Value);
			strScale.erase(0, dot + 1);
		}

		Key = "Euler Angle";
		string strAngle = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 vRotate{};
		while (true)
		{
			if (strAngle.find(',') == string::npos)
			{
				Value = strAngle.substr(0, strAngle.size());
				*(((float*)&vRotate) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strAngle.find(',');
			Value = strAngle.substr(0, dot);
			*(((float*)&vRotate) + (PointerSize++)) = stof(Value);
			strAngle.erase(0, dot + 1);
		}

		Key = "Position";
		string strPos = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 Position{};
		while (true)
		{
			if (strPos.find(',') == string::npos)
			{
				Value = strPos.substr(0, strPos.size());
				*(((float*)&Position) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strPos.find(',');
			Value = strPos.substr(0, dot);
			*(((float*)&Position) + (PointerSize++)) = stof(Value);
			strPos.erase(0, dot + 1);
		}


		pItem->getTransform()->setScale(Scale);
		pItem->getTransform()->setAngle(vRotate);
		pItem->getTransform()->setPos(Position);
		pItem->setActive(true);

		Key = "Radius";
		std::string strRadius = m_pIniManager->LoadDataString(strFile, Section, Key);
		_float fRadius = stof(strRadius);
		static_cast<CSphereCollision*>(pItem->getCollider())->setRadius(fRadius);

		Add_GameObject(LAYERID::LOADING, GAMEOBJECTID::ITEM, pItem);
	}
	return S_OK;
}

HRESULT CLoading::Load_MeleeMon(const _tchar* strName)
{
	wstring tmp = strName;
	string strFile(tmp.begin(), tmp.end());


	int MeleeMonSize = m_pIniManager->LoadDataInteger(strFile, "MeleeMonCount", "Count");

	string Section;
	string Key;
	string Value;
	CMeleeMon* pMeleeMon = nullptr;

	for (int i = 0; i < MeleeMonSize; ++i)
	{
		Section = string_format("MeleeMon_%d", i);
		size_t dot = 0;
		int PointerSize = 0;

		pMeleeMon = Clone_ObjProto<CMeleeMon>(GAMEOBJECTID::MONSTER1);

		Key = "ObjectAndTypeName";

		std::string FileFolderName = m_pIniManager->LoadDataString(strFile, Section, Key);
		std::wstring ObjectName;
		std::wstring TypeName;
		while (true)
		{
			if (FileFolderName.find(',') == std::string::npos)
			{
				Value = FileFolderName.substr(0, FileFolderName.size());
				TypeName.assign(Value.begin(), Value.end());
				break;
			}
			dot = FileFolderName.find(',');
			Value = FileFolderName.substr(0, dot);
			ObjectName.assign(Value.begin(), Value.end());
			FileFolderName.erase(0, dot + 1);
		}

		pMeleeMon->SettingAnimator();

		Key = "Scale";
		string strScale = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 Scale{};

		while (true)
		{
			if (strScale.find(',') == string::npos)
			{
				Value = strScale.substr(0, strScale.size());
				*(((float*)&Scale) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strScale.find(',');
			Value = strScale.substr(0, dot);
			*(((float*)&Scale) + (PointerSize++)) = stof(Value);
			strScale.erase(0, dot + 1);
		}

		Key = "Euler Angle";
		string strAngle = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 vRotate{};
		while (true)
		{
			if (strAngle.find(',') == string::npos)
			{
				Value = strAngle.substr(0, strAngle.size());
				*(((float*)&vRotate) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strAngle.find(',');
			Value = strAngle.substr(0, dot);
			*(((float*)&vRotate) + (PointerSize++)) = stof(Value);
			strAngle.erase(0, dot + 1);
		}

		Key = "Position";
		string strPos = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 Position{};
		while (true)
		{
			if (strPos.find(',') == string::npos)
			{
				Value = strPos.substr(0, strPos.size());
				*(((float*)&Position) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strPos.find(',');
			Value = strPos.substr(0, dot);
			*(((float*)&Position) + (PointerSize++)) = stof(Value);
			strPos.erase(0, dot + 1);
		}


		pMeleeMon->getTransform()->setScale(Scale);
		pMeleeMon->getTransform()->setAngle(vRotate);
		pMeleeMon->getTransform()->setPos(Position);
		pMeleeMon->setActive(true);

		Key = "Radius";
		std::string strRadius = m_pIniManager->LoadDataString(strFile, Section, Key);
		_float fRadius = stof(strRadius);
		static_cast<CSphereCollision*>(pMeleeMon->getCollider())->setRadius(fRadius);

		Add_GameObject(LAYERID::LOADING, GAMEOBJECTID::ITEM, pMeleeMon);
	}
	return S_OK;
}

HRESULT CLoading::Load_ShootMon(const _tchar* strName)
{
	wstring tmp = strName;
	string strFile(tmp.begin(), tmp.end());


	int ShootMonSize = m_pIniManager->LoadDataInteger(strFile, "ShootMonCount", "Count");

	string Section;
	string Key;
	string Value;
	CShootMon* pShootMon = nullptr;

	for (int i = 0; i < ShootMonSize; ++i)
	{
		Section = string_format("ShootMon_%d", i);
		size_t dot = 0;
		int PointerSize = 0;

		pShootMon = Clone_ObjProto<CShootMon>(GAMEOBJECTID::MONSTER2);

		Key = "ObjectAndTypeName";

		std::string FileFolderName = m_pIniManager->LoadDataString(strFile, Section, Key);
		std::wstring ObjectName;
		std::wstring TypeName;
		while (true)
		{
			if (FileFolderName.find(',') == std::string::npos)
			{
				Value = FileFolderName.substr(0, FileFolderName.size());
				TypeName.assign(Value.begin(), Value.end());
				break;
			}
			dot = FileFolderName.find(',');
			Value = FileFolderName.substr(0, dot);
			ObjectName.assign(Value.begin(), Value.end());
			FileFolderName.erase(0, dot + 1);
		}

		pShootMon->SettingAnimator();

		Key = "Scale";
		string strScale = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 Scale{};

		while (true)
		{
			if (strScale.find(',') == string::npos)
			{
				Value = strScale.substr(0, strScale.size());
				*(((float*)&Scale) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strScale.find(',');
			Value = strScale.substr(0, dot);
			*(((float*)&Scale) + (PointerSize++)) = stof(Value);
			strScale.erase(0, dot + 1);
		}

		Key = "Euler Angle";
		string strAngle = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 vRotate{};
		while (true)
		{
			if (strAngle.find(',') == string::npos)
			{
				Value = strAngle.substr(0, strAngle.size());
				*(((float*)&vRotate) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strAngle.find(',');
			Value = strAngle.substr(0, dot);
			*(((float*)&vRotate) + (PointerSize++)) = stof(Value);
			strAngle.erase(0, dot + 1);
		}

		Key = "Position";
		string strPos = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 Position{};
		while (true)
		{
			if (strPos.find(',') == string::npos)
			{
				Value = strPos.substr(0, strPos.size());
				*(((float*)&Position) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strPos.find(',');
			Value = strPos.substr(0, dot);
			*(((float*)&Position) + (PointerSize++)) = stof(Value);
			strPos.erase(0, dot + 1);
		}


		pShootMon->getTransform()->setScale(Scale);
		pShootMon->getTransform()->setAngle(vRotate);
		pShootMon->getTransform()->setPos(Position);
		pShootMon->setActive(true);

		Key = "Radius";
		std::string strRadius = m_pIniManager->LoadDataString(strFile, Section, Key);
		_float fRadius = stof(strRadius);
		static_cast<CSphereCollision*>(pShootMon->getCollider())->setRadius(fRadius);

		Add_GameObject(LAYERID::LOADING, GAMEOBJECTID::ITEM, pShootMon);
	}
	return S_OK;
}

HRESULT CLoading::Load_FlyMon(const _tchar* strName)
{
	wstring tmp = strName;
	string strFile(tmp.begin(), tmp.end());


	int FlyMonSize = m_pIniManager->LoadDataInteger(strFile, "FlyMonCount", "Count");

	string Section;
	string Key;
	string Value;
	CFlyMon* pFlyMon = nullptr;

	for (int i = 0; i < FlyMonSize; ++i)
	{
		Section = string_format("FlyMon_%d", i);
		size_t dot = 0;
		int PointerSize = 0;

		pFlyMon = Clone_ObjProto<CFlyMon>(GAMEOBJECTID::MONSTER3);

		Key = "ObjectAndTypeName";

		std::string FileFolderName = m_pIniManager->LoadDataString(strFile, Section, Key);
		std::wstring ObjectName;
		std::wstring TypeName;
		while (true)
		{
			if (FileFolderName.find(',') == std::string::npos)
			{
				Value = FileFolderName.substr(0, FileFolderName.size());
				TypeName.assign(Value.begin(), Value.end());
				break;
			}
			dot = FileFolderName.find(',');
			Value = FileFolderName.substr(0, dot);
			ObjectName.assign(Value.begin(), Value.end());
			FileFolderName.erase(0, dot + 1);
		}

		pFlyMon->SettingAnimator();

		Key = "Scale";
		string strScale = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 Scale{};

		while (true)
		{
			if (strScale.find(',') == string::npos)
			{
				Value = strScale.substr(0, strScale.size());
				*(((float*)&Scale) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strScale.find(',');
			Value = strScale.substr(0, dot);
			*(((float*)&Scale) + (PointerSize++)) = stof(Value);
			strScale.erase(0, dot + 1);
		}

		Key = "Euler Angle";
		string strAngle = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 vRotate{};
		while (true)
		{
			if (strAngle.find(',') == string::npos)
			{
				Value = strAngle.substr(0, strAngle.size());
				*(((float*)&vRotate) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strAngle.find(',');
			Value = strAngle.substr(0, dot);
			*(((float*)&vRotate) + (PointerSize++)) = stof(Value);
			strAngle.erase(0, dot + 1);
		}

		Key = "Position";
		string strPos = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 Position{};
		while (true)
		{
			if (strPos.find(',') == string::npos)
			{
				Value = strPos.substr(0, strPos.size());
				*(((float*)&Position) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strPos.find(',');
			Value = strPos.substr(0, dot);
			*(((float*)&Position) + (PointerSize++)) = stof(Value);
			strPos.erase(0, dot + 1);
		}


		pFlyMon->getTransform()->setScale(Scale);
		pFlyMon->getTransform()->setAngle(vRotate);
		pFlyMon->getTransform()->setPos(Position);
		pFlyMon->setActive(true);

		Key = "Radius";
		std::string strRadius = m_pIniManager->LoadDataString(strFile, Section, Key);
		_float fRadius = stof(strRadius);
		static_cast<CSphereCollision*>(pFlyMon->getCollider())->setRadius(fRadius);

		Add_GameObject(LAYERID::LOADING, GAMEOBJECTID::ITEM, pFlyMon);
	}
	return S_OK;
}

HRESULT	CLoading::Load_Monster(const _tchar* strName)
{
	wstring tmp = strName;
	string strFile(tmp.begin(), tmp.end());


	int MonsterSize = m_pIniManager->LoadDataInteger(strFile, "MonsterCount", "Count");

	string Section;
	string Key;
	string Value;
	CMeleeMon* pMeleeMon = nullptr;
	CShootMon* pShootMon = nullptr;
	CFlyMon* pFlyMon = nullptr;

	//////////////////////////////////
	for (int i = 0; i < MonsterSize; ++i)
	{
		Section = string_format("FlyMon_%d", i);
		size_t dot = 0;
		int PointerSize = 0;

		pFlyMon = Clone_ObjProto<CFlyMon>(GAMEOBJECTID::MONSTER3);

		Key = "ObjectAndTypeName";

		std::string FileFolderName = m_pIniManager->LoadDataString(strFile, Section, Key);
		std::wstring ObjectName;
		std::wstring TypeName;
		while (true)
		{
			if (FileFolderName.find(',') == std::string::npos)
			{
				Value = FileFolderName.substr(0, FileFolderName.size());
				TypeName.assign(Value.begin(), Value.end());
				break;
			}
			dot = FileFolderName.find(',');
			Value = FileFolderName.substr(0, dot);
			ObjectName.assign(Value.begin(), Value.end());
			FileFolderName.erase(0, dot + 1);
		}
		//////////////////////////////////

		pMeleeMon->SettingAnimator();
		pShootMon->SettingAnimator();
		pFlyMon->SettingAnimator();

		Key = "Scale";
		string strScale = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 Scale{};

		while (true)
		{
			if (strScale.find(',') == string::npos)
			{
				Value = strScale.substr(0, strScale.size());
				*(((float*)&Scale) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strScale.find(',');
			Value = strScale.substr(0, dot);
			*(((float*)&Scale) + (PointerSize++)) = stof(Value);
			strScale.erase(0, dot + 1);
		}

		Key = "Euler Angle";
		string strAngle = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 vRotate{};
		while (true)
		{
			if (strAngle.find(',') == string::npos)
			{
				Value = strAngle.substr(0, strAngle.size());
				*(((float*)&vRotate) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strAngle.find(',');
			Value = strAngle.substr(0, dot);
			*(((float*)&vRotate) + (PointerSize++)) = stof(Value);
			strAngle.erase(0, dot + 1);
		}

		Key = "Position";
		string strPos = m_pIniManager->LoadDataString(strFile, Section, Key);

		_vec3 Position{};
		while (true)
		{
			if (strPos.find(',') == string::npos)
			{
				Value = strPos.substr(0, strPos.size());
				*(((float*)&Position) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strPos.find(',');
			Value = strPos.substr(0, dot);
			*(((float*)&Position) + (PointerSize++)) = stof(Value);
			strPos.erase(0, dot + 1);
		}


		pMeleeMon->getTransform()->setScale(Scale);
		pMeleeMon->getTransform()->setAngle(vRotate);
		pMeleeMon->getTransform()->setPos(Position);
		pMeleeMon->setActive(true);

		pShootMon->getTransform()->setScale(Scale);
		pShootMon->getTransform()->setAngle(vRotate);
		pShootMon->getTransform()->setPos(Position);
		pShootMon->setActive(true);

		pFlyMon->getTransform()->setScale(Scale);
		pFlyMon->getTransform()->setAngle(vRotate);
		pFlyMon->getTransform()->setPos(Position);
		pFlyMon->setActive(true);

		Key = "Radius";
		std::string strRadius = m_pIniManager->LoadDataString(strFile, Section, Key);
		_float fRadius = stof(strRadius);
		static_cast<CSphereCollision*>(pMeleeMon->getCollider())->setRadius(fRadius);
		static_cast<CSphereCollision*>(pShootMon->getCollider())->setRadius(fRadius);
		static_cast<CSphereCollision*>(pFlyMon->getCollider())->setRadius(fRadius);

		Add_GameObject(LAYERID::LOADING, GAMEOBJECTID::MONSTER1, pMeleeMon);
		Add_GameObject(LAYERID::LOADING, GAMEOBJECTID::MONSTER2, pShootMon);
		Add_GameObject(LAYERID::LOADING, GAMEOBJECTID::MONSTER3, pFlyMon);
	}
	return S_OK;
}

HRESULT CLoading::Load_Boss(const _tchar* strName)
{
	return S_OK;
}