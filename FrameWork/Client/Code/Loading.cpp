#include "pch.h"
#include "Loading.h"
#include "BackGround.h"
#include "MeleeMon.h"
#include "Player.h"
#include "Terrain.h"
#include "MainCamera.h"
#include "PlayerModel.h"
#include "Animator.h"
#include "Collision.h"
#include "ShootMon.h"
#include "FlyMon.h"
#include "Shuriken.h"
#include "Bomb.h"
#include "MonBullet.h"
#include "Cube.h"
#include "Quad.h"
#include "Fog.h"

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

_uint CLoading::Loading_ForStage()
{
	//ToDo:쓰레드로 불러들일 곳
	m_pTextureMgr = Init_TextureMgr();
	NULL_CHECK_RETURN(m_pTextureMgr, -1);

	//Texture불러오기
	/*m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/MeleeMon/Idle/IDLE_000.png", L"MeleeMon_Idle", 1);
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
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Monster/FlyMon/Death/DEATH_00%d.png", L"FlyMon_Death", 7);*/

	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Attack/Sword/Player_Attack_Sword00%d.png", L"PlayerSwordAttack", 4);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Attack/Shuriken/Player_Attack_Shuriken00%d.png", L"PlayerShurikenAttack",8);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Attack/Bomb/Player_Attack_Bomb00%d.png", L"PlayerBombAttack",3);

	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Shuriken/Shuriken00%d.png", L"Shuriken", 4);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Bomb/Bomb00%d.png", L"Bomb", 2);
	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Bomb/BombSmoge00%d.png", L"BombSmoge", 4);

	m_pTextureMgr->Insert_Texture(m_pDevice, TEXTURETYPE::TEX_NORMAL, L"../Bin/Resource/Test/monster.png", L"Monster", 1);
	
	//Component원본 생성
	CComponent* pCom = nullptr;

	pCom = CRcTex::Create(m_pDevice);
	NULL_CHECK_RETURN(pCom, -1);
	Init_ComProto(COMPONENTID::RCTEX, pCom);

	pCom = CTexture::Create(m_pDevice);
	NULL_CHECK_RETURN(pCom, -1);
	Init_ComProto(COMPONENTID::TEXTURE, pCom);

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

	//Quad
	pObj = CQuad::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::QUAD, pObj);

	//Cube
	pObj = CCube::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::CUBE, pObj);

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
	
	//// Melee Monster #1
	//pObj = CMeleeMon::Create(m_pDevice);
	//NULL_CHECK_RETURN(pObj, -1);
	//Init_ObjProto(GAMEOBJECTID::MONSTER1, pObj);
	//
	//// Shoot Monster
	//pObj = CShootMon::Create(m_pDevice);
	//NULL_CHECK_RETURN(pObj, -1);
	//Init_ObjProto(GAMEOBJECTID::MONSTER2, pObj);

	//// MonBullet
	//pObj = CMonBullet::Create(m_pDevice);
	//NULL_CHECK_RETURN(pObj, -1);
	//Init_ObjProto(GAMEOBJECTID::MONBULLET, pObj);
	//
	//// Fly Monster
	//pObj = CFlyMon::Create(m_pDevice);
	//NULL_CHECK_RETURN(pObj, -1);
	//Init_ObjProto(GAMEOBJECTID::MONSTER3, pObj);

	pObj = CShuriken::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::SHURIKEN, pObj);

	pObj = CBomb::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::BOMB, pObj);

	pObj = CFog::Create(m_pDevice);
	NULL_CHECK_RETURN(pObj, -1);
	Init_ObjProto(GAMEOBJECTID::FOG, pObj);
	
	FAILED_CHECK_RETURN(Load_Terrain(L"TerrainData"),E_FAIL);
	FAILED_CHECK_RETURN(Load_Quad(L"QuadData"),E_FAIL);
	//FAILED_CHECK_RETURN(Load_Cube(L"CubeData"),E_FAIL);

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

HRESULT CLoading::Load_Terrain(const _tchar* strName)
{
	wstring tmp = strName;
	string strFile(tmp.begin(), tmp.end());
	string strtmp = strFile;
	int TerrainSize = m_pIniManager->LoadDataInteger(strtmp, "TerrainCount", "Count");
	
	string Section;
	string Key;
	string Value;
	CTerrain* pTerrain = nullptr;

	for (int i = 0; i < TerrainSize; ++i)
	{
		Section = string_format("Terrain_%d", i);
		Key = "Info";

		strtmp = strFile;
		string VtxInfoValue = m_pIniManager->LoadDataString(strtmp, Section, Key);

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


		CTerrainTex* pTerrainTex = CTerrainTex::Create(m_pDevice, iVecTerrainInfo[0]
			, iVecTerrainInfo[1], iVecTerrainInfo[2], iVecTerrainInfo[3]);
		NULL_CHECK_RETURN(pTerrainTex, -1);
		Init_ComProto(COMPONENTID::TERRAINTEX, pTerrainTex);

		CGameObject* pObj = CTerrain::Create(m_pDevice);
		NULL_CHECK_RETURN(pObj);
		Init_ObjProto(GAMEOBJECTID::TERRAIN, pObj);

		pTerrain = Clone_ObjProto<CTerrain>(GAMEOBJECTID::TERRAIN);

		//지형 텍스쳐 불러오기
		wstring FolderName;
		wstring FileName;

		Key = "FileFolderName";
		strtmp = strFile;
		string ObjectAndTypeName = m_pIniManager->LoadDataString(strtmp, Section, Key);

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
		strtmp = strFile;
		string strScale = m_pIniManager->LoadDataString(strtmp, Section, Key);

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
		strtmp = strFile;
		string strAngle = m_pIniManager->LoadDataString(strtmp, Section, Key);

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
		strtmp = strFile;
		string strPos = m_pIniManager->LoadDataString(strtmp, Section, Key);

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
		Add_GameObject(LAYERID::LOADING, GAMEOBJECTID::TERRAIN, pTerrain);
	}
	return S_OK;
}

HRESULT CLoading::Load_Quad(const _tchar* strName)
{
	wstring tmp = strName;
	string strFile(tmp.begin(), tmp.end());
	string strtmp = strFile;
	int QuadSize = m_pIniManager->LoadDataInteger(strtmp, "QuadCount", "Count");

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
		strtmp = strFile;
		string ObjectAndTypeName = m_pIniManager->LoadDataString(strtmp, Section, Key);

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
		strtmp = strFile;
		string strScale = m_pIniManager->LoadDataString(strtmp, Section, Key);

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
		strtmp = strFile;
		string strAngle = m_pIniManager->LoadDataString(strtmp, Section, Key);

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
		strtmp = strFile;
		string strPos = m_pIniManager->LoadDataString(strtmp, Section, Key);

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
		pQuad->LoadTransform(Scale, Roatate, Position);
		pQuad->setActive(false);
		Add_GameObject(LAYERID::LOADING, GAMEOBJECTID::QUAD, pQuad);
	}
	return S_OK;
}

HRESULT CLoading::Load_Cube(const _tchar* strName)
{
	wstring tmp = strName;
	string strFile(tmp.begin(), tmp.end());
	string strtmp = strFile;

	int QuadSize = m_pIniManager->LoadDataInteger(strtmp, "CubeCount", "Count");

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
		strtmp = strFile;
		string ObjectAndTypeName = m_pIniManager->LoadDataString(strtmp, Section, Key);
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
		strtmp = strFile;
		string strScale = m_pIniManager->LoadDataString(strtmp, Section, Key);

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
		strtmp = strFile;
		string strAngle = m_pIniManager->LoadDataString(strtmp, Section, Key);

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
		strtmp = strFile;
		string strPos = m_pIniManager->LoadDataString(strtmp, Section, Key);

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

		pCube->LoadTransform(Scale, Roatate, Position);
		pCube->setActive(false);
		Add_GameObject(LAYERID::LOADING, GAMEOBJECTID::CUBE, pCube);
	}
	return S_OK;
}
