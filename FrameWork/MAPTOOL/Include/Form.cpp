// Form.cpp: 구현 파일
//

#include "pch.h"
#include "MAPTOOL.h"
#include "Form.h"
#include "MAPTOOLView.h"
#include "MainFrm.h"

#include "VIBuffer.h"
#include "TerrainObject.h"
#include "QuadObject.h"
#include "CubeObject.h"
#include "ToolGameObject.h"

#include "INIManager.h"

// CForm

IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_CForm)
	, m_strFolderName(_T(""))
	, m_strFileName(_T(""))
	, m_TerrainlistIndex(0)
	, m_pMapToolView(nullptr)
	, m_iNewTerrainX(0)
	, m_iNewTerrainZ(0)
	, m_iNewTerrainInterval(0)
	, m_fMovePower(0)
	, m_strTreeFilterName(_T(""))
	, m_strObjectName(_T(""))
{
	ZeroMemory(&m_tPlayerPos, sizeof(_float) * 2);
}

CForm::~CForm()
{
	INIManager::DestroyInstance();
}

void CForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, Terrain_dwCntX, m_tNowInfo.X);
	DDX_Text(pDX, Terrain_dwCntZ, m_tNowInfo.Z);
	DDX_Text(pDX, Terrain_DwInterval, m_tNowInfo.Interval);
	DDX_Text(pDX, Terrain_DeTail, m_tNowInfo.Detail);
	DDX_Control(pDX, Terrain_WireFrame, m_bWireFrame);
	DDX_Control(pDX, List_Terrain, m_List_Terrain);
	DDX_Text(pDX, Player_PosX, m_tPlayerPos[0]);
	DDX_Text(pDX, Player_PosY, m_tPlayerPos[1]);
	DDX_Text(pDX, Terrain_FolderName, m_strFolderName);
	DDX_Text(pDX, Terrain_FileName, m_strFileName);
	DDX_Text(pDX, Object_ScaleX, m_vecScale.x);
	DDX_Text(pDX, Object_ScaleY, m_vecScale.y);
	DDX_Text(pDX, Object_ScaleZ, m_vecScale.z);
	DDX_Text(pDX, Object_RotX, m_vecRotaion.x);
	DDX_Text(pDX, Object_RotY, m_vecRotaion.y);
	DDX_Text(pDX, Object_RotZ, m_vecRotaion.z);
	DDX_Text(pDX, Object_PosX, m_vecPosition.x);
	DDX_Text(pDX, Object_PosY, m_vecPosition.y);
	DDX_Text(pDX, Object_PosZ, m_vecPosition.z);
	DDX_Text(pDX, Terrain_NewX, m_iNewTerrainX);
	DDX_Text(pDX, Terrain_NewZ, m_iNewTerrainZ);
	DDX_Text(pDX, Terrain_NewInterval, m_iNewTerrainInterval);
	DDX_Control(pDX, Radio_Scale, m_Button_Scale);
	DDX_Control(pDX, Radio_Rotation, m_Button_Roation);
	DDX_Control(pDX, Radio_Position, m_Button_Position);
	DDX_Text(pDX, Object_MovePower, m_fMovePower);
	DDX_Control(pDX, Button_ZBuffer, m_Button_Zbuffer);
	DDX_Control(pDX, Tree_Object, m_Tree_Object);
	DDX_Text(pDX, Edit_Tree_FilterName, m_strTreeFilterName);
	DDX_Text(pDX, Edit_ObjectName, m_strObjectName);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)
	ON_BN_CLICKED(Terrain_CreateButton, &CForm::OnBnClickedCreatebutton)
	ON_BN_CLICKED(Terrain_Texture, &CForm::OnBnClickedTexture)
	ON_NOTIFY(UDN_DELTAPOS, Terrain_DetailSpin, &CForm::OnDeltaposDetailspin)
	ON_BN_CLICKED(BUTTON_Terrain_Save, &CForm::OnBnClickedTerrainSave)
	ON_BN_CLICKED(BUTTON_Terrain_Load, &CForm::OnBnClickedTerrainLoad)
	ON_EN_CHANGE(Terrain_DeTail, &CForm::OnEnChangeDetail)
	ON_LBN_SELCHANGE(List_Terrain, &CForm::OnLbnSelchangeTerrain)
	ON_BN_CLICKED(Terrain_ModifyButton, &CForm::OnBnClickedModifybutton)
	ON_LBN_SETFOCUS(List_Terrain, &CForm::OnLbnSetfocusTerrain)
	ON_EN_CHANGE(Object_MovePower, &CForm::OnEnChangeMovepower)
	ON_BN_CLICKED(Button_Quad_Create, &CForm::OnBnClickedQuadCreate)
	ON_BN_CLICKED(Button_Cube_Create, &CForm::OnBnClickedCubeCreate)
	ON_BN_CLICKED(Button_MakeFilter, &CForm::OnBnClickedMakefilter)
	ON_BN_CLICKED(Button_DeleteFilter, &CForm::OnBnClickedDeletefilter)
	ON_BN_CLICKED(Button_ModifyFilter, &CForm::OnBnClickedModifyfilter)
	ON_WM_MOUSEWHEEL()
	ON_NOTIFY(TVN_SELCHANGED, Tree_Object, &CForm::OnTvnSelchangedObject)
	ON_NOTIFY(NM_SETFOCUS, Tree_Object, &CForm::OnNMSetfocusObject)
	ON_LBN_KILLFOCUS(List_Terrain, &CForm::OnLbnKillfocusTerrain)
	ON_BN_CLICKED(Button_Clone_Object, &CForm::OnBnClickedCloneObject)
	ON_BN_CLICKED(BUTTON_Object_Save, &CForm::OnBnClickedObjectSave)
	ON_BN_CLICKED(BUTTON_Object_Load, &CForm::OnBnClickedObjectLoad)
	ON_BN_CLICKED(BUTTON_Cube_save, &CForm::OnBnClickedCubesave)
	ON_BN_CLICKED(BUTTON_Cube_Load, &CForm::OnBnClickedCubeLoad)
END_MESSAGE_MAP()


// CForm 진단

#ifdef _DEBUG
void CForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForm 메시지 처리기


void CForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	SetScrollSizes(MM_TEXT, CSize(0, 0));

	// View창 연결하기
	m_pMapToolView = dynamic_cast<CMAPTOOLView*>(dynamic_cast<CMainFrame*>(AfxGetMainWnd())->m_tMainSplitter.GetPane(0, 1));

	// 기본 Detail값
	m_iNewTerrainInterval = 1;

	// 기본 vector값
	m_vecScale = { 1.f,1.f,1.f };
	m_vecRotaion = { 0.f,0.f,0.f };
	m_vecPosition = { 0.f,0.f,0.f };

	m_fMovePower = 1.f;

	m_TreeRoot = m_Tree_Object.InsertItem(L"Object", 0, 0, TVI_ROOT, TVI_LAST);

	m_pIniManager = INIManager::GetInstance();
	
}


void CForm::OnBnClickedCreatebutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	VTXINFO tTerrainInfo;
	tTerrainInfo.X = m_iNewTerrainX;
	tTerrainInfo.Z = m_iNewTerrainZ;
	tTerrainInfo.Interval = m_iNewTerrainInterval;
	tTerrainInfo.Detail = 1;

	if (!tTerrainInfo.X || !tTerrainInfo.Z)
		return;

	CGameObject* pObj = CTerrainObject::Create(m_pMapToolView->m_pDevice, tTerrainInfo);
	static_cast<CToolGameObject*>(pObj)->Set_TypeName(L"Terrain");

	CString Indextemp;
	Indextemp.Format(L"Terrain_%d", m_TerrainlistIndex);
	m_List_Terrain.AddString(Indextemp);
	m_List_Terrain.SetCurSel(m_TerrainlistIndex);
	++m_TerrainlistIndex;
	//리소스를 변수로 보낸다.
	static_cast<CToolGameObject*>(pObj)->Set_ObjectName(Indextemp);
	m_pMapToolView->m_listTerrain.emplace_back(pObj);
	UpdateData(true);

	m_iNewTerrainX = 0;
	m_iNewTerrainZ = 0;
	m_iNewTerrainInterval = 1;
	//변수들을 리소스로 보낸다.
	m_List_Terrain.SetFocus();
	UpdateData(false);


	GetDlgItem(Terrain_dwCntX)->EnableWindow(TRUE);
	GetDlgItem(Terrain_dwCntZ)->EnableWindow(TRUE);
	GetDlgItem(Terrain_DwInterval)->EnableWindow(TRUE);
	GetDlgItem(Terrain_DeTail)->EnableWindow(TRUE);
	GetDlgItem(Terrain_DetailSpin)->EnableWindow(TRUE);


}


void CForm::OnBnClickedTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_tTerrainTexture.GetSafeHwnd())
		m_tTerrainTexture.Create(IDD_CTerrainTexture);
	m_tTerrainTexture.ShowWindow(SW_SHOW);
}

void CForm::OnDeltaposDetailspin(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (pNMUpDown->iDelta < 0)
	{
		dynamic_cast<CToolGameObject*>(m_pNowObject)->Set_Detail(++m_tNowInfo.Detail);
		//++m_tNowInfo.Detail;
	}
	else
	{
		dynamic_cast<CToolGameObject*>(m_pNowObject)->Set_Detail(--m_tNowInfo.Detail);

		//--m_tNowInfo.Detail;
	}

	// false = 우리가눈으로 보여지는 리소스로 코드안에있는 변수들을내보냄
	// true = 리소스에있는 데이터들을 코드안에있는 변수들에게 집어넣음
	UpdateData(false);
	//CString strTypeName;
	//static_cast<CToolGameObject*>(m_pNowObject)->Get_TypeName(strTypeName);

	//	if (strTypeName == L"Terrain")
	//	{
	//		ReSize_Detail();
	//	}
	ReSize_Detail();
	*pResult = 0;
}


void CForm::OnBnClickedTerrainSave()
{
	std::string section;
	std::string Key;
	std::string Value;

	section = "TerrainCount";

	Key = "Count";

	Value =  to_string(m_pMapToolView->m_listTerrain.size());

	m_pIniManager->AddData(section, Key, Value);

	int i = 0;

	for (const auto& Terrain : m_pMapToolView->m_listTerrain)
	{
		section = string_format("Terrain_%d",i++);
		Key = "Info";

		VTXINFO* NowTerrainInfo = nullptr;
		static_cast<CToolGameObject*>(Terrain)->Get_VTXINFO(&NowTerrainInfo); 
		Value = string_format("%d,%d,%d,%d", NowTerrainInfo->X, NowTerrainInfo->Z, NowTerrainInfo->Interval, NowTerrainInfo->Detail);

		m_pIniManager->AddData(section, Key, Value);

		Key = "FileFolderName";

		const std::vector<TEXTUREINFO>& vecTextureInfo = static_cast<CToolGameObject*>(Terrain)->Get_vecTextureInfo();
		for (const auto& TextureInfo : vecTextureInfo)
		{
			string folderName{ TextureInfo.strTextureFolder.begin(),TextureInfo.strTextureFolder.end() };
			string FileName{ TextureInfo.strTextureName.begin(),TextureInfo.strTextureName.end() };
			Value = folderName + "," + FileName;

			m_pIniManager->AddData(section, Key, Value);
		}


		Key = "ObjectAndTypeName";
		CString strObjectName;
		static_cast<CToolGameObject*>(Terrain)->Get_ObjectName(strObjectName);
		string ObjectName = std::string(CT2CA(strObjectName));

		CString strTypeName;
		static_cast<CToolGameObject*>(Terrain)->Get_TypeName(strTypeName);
		string TypeName = std::string(CT2CA(strTypeName));

		Value = ObjectName + "," + TypeName;
		
		m_pIniManager->AddData(section, Key, Value);
		
		Key = "Scale";
		const _vec3& vec3Scale = Terrain->getTransform()->getScale();
		Value = string_format("%f,%f,%f", vec3Scale.x, vec3Scale.y, vec3Scale.z);
		m_pIniManager->AddData(section, Key, Value);

		Key = "Euler Angle";
		const _vec3& pVec = Terrain->getTransform()->getToolAngle();
		Value = string_format("%f,%f,%f", pVec.x, pVec.y, pVec.z);
		m_pIniManager->AddData(section, Key, Value);

		Key = "Position";
		const _vec3& vec3Position = Terrain->getTransform()->getPos();
		Value = string_format("%f,%f,%f", vec3Position.x, vec3Position.y, vec3Position.z);
		m_pIniManager->AddData(section, Key, Value);


	}

	m_pIniManager->SaveIni(std::string("TerrainData"));
}


void CForm::OnBnClickedTerrainLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int TerrainSize = m_pIniManager->LoadDataInteger(std::string("TerrainData"), "TerrainCount", "Count");
	if (!m_pMapToolView->m_listTerrain.empty())
	{
		std::for_each(m_pMapToolView->m_listTerrain.begin(), m_pMapToolView->m_listTerrain.end(), DeleteObj);
		m_pMapToolView->m_listTerrain.clear();
		m_List_Terrain.ResetContent();
		m_TerrainlistIndex = 0;
	}

	std::string Section;
	std::string Key;
	std::string Value;
	CGameObject* pTerrain = nullptr;
	
	for (int i = 0; i < TerrainSize; ++i)
	{
		Section = string_format("Terrain_%d", i);
		Key = "Info";

		std::string VtxInfoValue = m_pIniManager->LoadDataString(std::string("TerrainData"), Section, Key);

		VTXINFO NowTerrainInfo ;

		size_t dot = 0;
		int PointerSize = 0;
		while (true)
		{
			if (VtxInfoValue.find(',') == std::string::npos)
			{
				Value = VtxInfoValue.substr(0, VtxInfoValue.size());
				*(((int*)&NowTerrainInfo) + (PointerSize)) = stoi(Value);
				PointerSize = 0;
				break;
			}
			dot = VtxInfoValue.find(',');
			Value = VtxInfoValue.substr(0, dot);
			*(((int*)&NowTerrainInfo) + (PointerSize++))  = stoi(Value);

			VtxInfoValue.erase(0, dot+1);
		}

		pTerrain = CTerrainObject::Create(m_pMapToolView->m_pDevice, NowTerrainInfo);

		std::wstring FolderName;
		std::wstring FileName;

		Key = "FileFolderName";


		std::string ObjectAndTypeName = m_pIniManager->LoadDataString(std::string("TerrainData"), Section, Key);

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

		dynamic_cast<CToolGameObject*>(pTerrain)->Set_Path(FolderName, FileName, 0);

		TCHAR strtemp[MAX_PATH] = L"..\\..\\Client\\Bin\\Resource\\Texture\\";
		lstrcat(strtemp, FolderName.c_str());
		lstrcat(strtemp, L"\\");
		lstrcat(strtemp, FileName.c_str());
		lstrcat(strtemp, L".png");
		if (!GetTexture(FileName.c_str(), TEXTURETYPE::TEX_NORMAL))
			Insert_Texture(m_pMapToolView->m_pGraphicDev->getDevice(), TEXTURETYPE::TEX_NORMAL, strtemp, FileName.c_str(), 1);

		std::vector<TEXTUREINFO> vecTextureInfo =static_cast<CToolGameObject*>(pTerrain)->Get_vecTextureInfo();

		vecTextureInfo[0].pTexture = CTexture::Create(m_pMapToolView->m_pDevice);
		vecTextureInfo[0].pTexture->setTexture(GetTexture(FileName.c_str(), TEXTURETYPE::TEX_NORMAL));

		static_cast<CToolGameObject*>(pTerrain)->Set_vecTextureInfo(vecTextureInfo);

		std::wstring ObjectName;
		std::wstring TypeName;

		Key = "ObjectAndTypeName";

		std::string FileFolderName = m_pIniManager->LoadDataString(std::string("TerrainData"), Section, Key);

		while (true)
		{
			if (FileFolderName.find(',') == std::string::npos)
			{
				Value = FileFolderName.substr(0, FileFolderName.size());
				TypeName.assign(Value.begin(),Value.end());
				break;
			}
			dot = FileFolderName.find(',');
			Value = FileFolderName.substr(0, dot);
			ObjectName.assign(Value.begin(),Value.end());
			FileFolderName.erase(0, dot + 1);
		}

		dynamic_cast<CToolGameObject*>(pTerrain)->Set_ObjectName(CString(ObjectName.c_str()));
		dynamic_cast<CToolGameObject*>(pTerrain)->Set_TypeName(CString(TypeName.c_str()));

		m_List_Terrain.AddString(ObjectName.c_str());
		m_List_Terrain.SetCurSel(m_TerrainlistIndex);
		++m_TerrainlistIndex;



		Key = "Scale";

		std::string strScale = m_pIniManager->LoadDataString(std::string("TerrainData"), Section, Key);

		_vec3 Scale{};

		while (true)
		{
			if (strScale.find(',') == std::string::npos)
			{
				Value = strScale.substr(0, strScale.size());
				*(((float*)&Scale) + (PointerSize)) = stof(Value);
				PointerSize = 0;
				break;
			}
			dot = strScale.find(',');
			Value = strScale.substr(0, dot);
			*( ((float*)&Scale) + (PointerSize++)) = stof(Value);
			strScale.erase(0, dot + 1);
		}

		Key = "Euler Angle";

		std::string strAngle = m_pIniManager->LoadDataString(std::string("TerrainData"), Section, Key);

		_vec3 Roatate{};
		while (true)
		{
			if (strAngle.find(',') == std::string::npos)
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

		std::string strPos = m_pIniManager->LoadDataString(std::string("TerrainData"), Section, Key);

		_vec3 Position{};
		while (true)
		{
			if (strPos.find(',') == std::string::npos)
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

		pTerrain->getTransform()->setScale(Scale);
		pTerrain->getTransform()->setAngle(Roatate);
		pTerrain->getTransform()->setPos(Position);

		m_pMapToolView->m_listTerrain.emplace_back(pTerrain);

		UpdateData(TRUE);
	}
}

void CForm::ReSize_Detail()
{
	VTXTEX* pVertex = nullptr;
	_ulong dwIndex = 0;
	CTerrainTex* pTerrainTex = dynamic_cast<CTerrainObject*>(m_pNowObject)->Get_Tex();

	pTerrainTex->Get_VBuffer()->Lock(0, 0, (void**)&pVertex, 0);
	UPOINT tPoint = pTerrainTex->Get_XZ();
	for (_ulong i = 0; i < tPoint.z; i++)
	{
		for (_ulong j = 0; j < tPoint.x; j++)
		{
			dwIndex = i * tPoint.x + j;
			pVertex[dwIndex].vUV = _vec2(_float(j) / (tPoint.x - 1) * m_tNowInfo.Detail, _float(i) / (tPoint.z - 1) * m_tNowInfo.Detail);
		}
	}
	pTerrainTex->Get_VBuffer()->Unlock();
}


void CForm::ReSize_ObjectInfo()
{
	VTXTEX* pVertex = nullptr;
	_ulong dwIndex = 0;

	CString strTypeName;
	static_cast<CToolGameObject*>(m_pNowObject)->Get_TypeName(strTypeName);

	if (strTypeName == L"Terrain")
	{
		CTerrainTex* pTerrainTex = dynamic_cast<CTerrainObject*>(m_pNowObject)->Get_Tex();
		if (dynamic_cast<CTerrainObject*>(m_pNowObject)->Compare_Info(&m_tNowInfo))
			return;
	
		pTerrainTex->Init_BufferNoTexture(m_tNowInfo.X, m_tNowInfo.Z, m_tNowInfo.Interval, m_tNowInfo.Detail);
	}
	else if (strTypeName == L"Quad")
	{
		CRcTex* pRcTex = static_cast<CQuadObject*>(m_pNowObject)->Get_Tex();
		if (static_cast<CQuadObject*>(m_pNowObject)->Compare_Info(&m_tNowInfo))
			return;
	
		pRcTex->Init_Buffer();
	}
	else if (strTypeName == L"Cube")
	{
		std::array<CRcTex*, 6> pCubeTex;
		static_cast<CCubeObject*>(m_pNowObject)->Get_arrTex(pCubeTex);
		if (static_cast<CQuadObject*>(m_pNowObject)->Compare_Info(&m_tNowInfo))
			return;

		for (_uint i = 0; i < pCubeTex.size(); ++i)
			pCubeTex[i]->Init_Buffer(i);
	}

	static_cast<CToolGameObject*>(m_pNowObject)->Set_VTXINFO(&m_tNowInfo);

	UpdateData(FALSE);


}

void CForm::LinkResourceAndVariableTerrain()
{
	if (m_pMapToolView->m_listTerrain.empty())
		return;

	int iCursel = m_List_Terrain.GetCurSel();

	//현재 선택된 Object가져옴
	auto& iter = m_pMapToolView->m_listTerrain.begin();
	for (int i = 0; i < iCursel; ++i)
		++iter;

	m_pNowObject = *(iter);

	// 현재Obj에 텍스처 정보
	static_cast<CToolGameObject*>(m_pNowObject)->Get_Path(m_strFolderName, m_strFileName);
	// 현재Obj에 Transform정보
	static_cast<CToolGameObject*>(m_pNowObject)->Linking_Transform(m_vecScale, m_vecRotaion, m_vecPosition);
	// 현재Obj에 TerrainInfo정보를 m_tNowInfo 에 연동시킴
	static_cast<CToolGameObject*>(m_pNowObject)->Linking_VTXINFO(&m_tNowInfo);

	UpdateData(false);
}

void CForm::LinkResourceAndVariableQuad()
{
	if (m_pMapToolView->m_listQuad.empty() && m_pMapToolView->m_listCube.empty())
		return;

	CString strNow = m_Tree_Object.GetItemText(m_TreeNow);

	_bool bFindCheck = false;

	for (const auto& iter : m_pMapToolView->m_listQuad)
	{
		if (dynamic_cast<CToolGameObject*>(iter)->Compare_Filter(strNow))
		{
			m_pNowObject = iter;
			bFindCheck = true;
			break;
		}
	}
	if (!bFindCheck)
	{
		for (const auto& iter : m_pMapToolView->m_listCube)
		{
			if (dynamic_cast<CToolGameObject*>(iter)->Compare_Filter(strNow))
			{
				m_pNowObject = iter;
				break;
			}
		}
	}
	if (!m_pNowObject)
		return;

	static_cast<CToolGameObject*>(m_pNowObject)->Get_Path(m_strFolderName, m_strFileName);
	static_cast<CToolGameObject*>(m_pNowObject)->Linking_Transform(m_vecScale, m_vecRotaion, m_vecPosition);
	static_cast<CToolGameObject*>(m_pNowObject)->Linking_VTXINFO(&m_tNowInfo);


	UpdateData(FALSE);

}

void CForm::Set_SRP(const _vec3& vecScale, const _vec3& vecRot, const _vec3& vecPos)
{
	m_vecScale = vecScale;
	m_vecRotaion = vecRot;
	m_vecPosition = vecPos;
	UpdateData(false);
}


void CForm::OnEnChangeDetail()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_tNowInfo.Detail <= 0)
		return;
	UpdateData(true);
	ReSize_Detail();
}

// Terrain List control
void CForm::OnLbnSelchangeTerrain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(Terrain_dwCntX)->EnableWindow(TRUE);
	GetDlgItem(Terrain_dwCntZ)->EnableWindow(TRUE);
	GetDlgItem(Terrain_DwInterval)->EnableWindow(TRUE);
	GetDlgItem(Terrain_DeTail)->EnableWindow(TRUE);
	GetDlgItem(Terrain_DetailSpin)->EnableWindow(TRUE);
	LinkResourceAndVariableTerrain();
}


void CForm::OnBnClickedModifybutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// X,Z,interval,Detail값을 연동시킨다.
	UpdateData(true);

	ReSize_ObjectInfo();
}

void CForm::OnLbnSetfocusTerrain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//LinkResourceAndVariableTerrain();
}


void CForm::OnEnChangeMovepower()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);
}

void CForm::OnBnClickedQuadCreate()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_strObjectName.IsEmpty())
		return;

	CGameObject* pObj = nullptr;
	pObj = CQuadObject::Create(m_pMapToolView->m_pDevice);
	static_cast<CToolGameObject*>(pObj)->Set_ObjectName(m_strObjectName);
	static_cast<CToolGameObject*>(pObj)->Set_TypeName(L"Quad");


	m_pMapToolView->m_listQuad.emplace_back(pObj);

	HTREEITEM m_TreeChild =  m_Tree_Object.InsertItem(m_strObjectName, m_TreeNow, TVI_LAST);

	m_Tree_Object.SelectItem(m_TreeChild);


	GetDlgItem(Terrain_dwCntX)->EnableWindow(FALSE);
	GetDlgItem(Terrain_dwCntZ)->EnableWindow(FALSE);
	GetDlgItem(Terrain_DwInterval)->EnableWindow(FALSE);
	GetDlgItem(Terrain_DeTail)->EnableWindow(FALSE);
	GetDlgItem(Terrain_DetailSpin)->EnableWindow(FALSE);
	
}


void CForm::OnBnClickedCubeCreate()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_strObjectName.IsEmpty())
		return;

	CGameObject* pObj = nullptr;
	pObj = CCubeObject::Create(m_pMapToolView->m_pDevice);
	static_cast<CToolGameObject*>(pObj)->Set_ObjectName(m_strObjectName);
	static_cast<CToolGameObject*>(pObj)->Set_TypeName(L"Cube");

	m_pMapToolView->m_listCube.emplace_back(pObj);

	HTREEITEM m_TreeChild = m_Tree_Object.InsertItem(m_strObjectName, m_TreeNow, TVI_LAST);

	m_Tree_Object.SelectItem(m_TreeChild);


	GetDlgItem(Terrain_dwCntX)->EnableWindow(FALSE);
	GetDlgItem(Terrain_dwCntZ)->EnableWindow(FALSE);
	GetDlgItem(Terrain_DwInterval)->EnableWindow(FALSE);
	GetDlgItem(Terrain_DeTail)->EnableWindow(FALSE);
	GetDlgItem(Terrain_DetailSpin)->EnableWindow(FALSE);

}


void CForm::OnBnClickedMakefilter()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_Tree_Object.InsertItem((LPCTSTR)m_strTreeFilterName, TVI_ROOT, TVI_LAST);
	m_Tree_Object.SetFocus();
}


void CForm::OnBnClickedDeletefilter()
{
	// TODO: Add your control notification handler code here
	CString str = m_Tree_Object.GetItemText(m_TreeNow);

	for (auto& iter = m_pMapToolView->m_listQuad.begin() ; iter != m_pMapToolView->m_listQuad.end() ; ++iter)
	{	
		
		if (dynamic_cast<CQuadObject*>(*iter)->Compare_Filter(str))
		{
			m_pMapToolView->m_listQuad.erase(iter);
			break;
		}
	}

	m_Tree_Object.DeleteItem(m_Tree_Object.GetSelectedItem());
	UpdateData(FALSE);
}


void CForm::OnBnClickedModifyfilter()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	HTREEITEM hTreeItem =  m_Tree_Object.GetSelectedItem();
	m_Tree_Object.SetItemText(hTreeItem, m_strTreeFilterName);
	
}


BOOL CForm::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{


	if (zDelta < 0 && m_fMovePower > 0)
		m_fMovePower *= -1;
	else if (zDelta > 0 && m_fMovePower < 0)
		m_fMovePower *= -1;

	if (m_pNowObject)
	{
		if (GetDlgItem(Object_PosX) == GetFocus())
		{
			m_vecPosition.x += m_fMovePower;
		}
		else if (GetDlgItem(Object_PosY) == GetFocus())
		{
			m_vecPosition.y += m_fMovePower;
		}
		else if (GetDlgItem(Object_PosZ) == GetFocus())
		{
			m_vecPosition.z += m_fMovePower;
		}
		else if (GetDlgItem(Object_ScaleX) == GetFocus())
		{
			m_vecScale.x += m_fMovePower;
		}
		else if (GetDlgItem(Object_ScaleY) == GetFocus())
		{
			m_vecScale.y += m_fMovePower;
		}
		else if (GetDlgItem(Object_ScaleZ) == GetFocus())
		{
			m_vecScale.z += m_fMovePower;
		}
		else if (GetDlgItem(Object_RotX) == GetFocus())
		{
			m_vecRotaion.x += m_fMovePower;
		}
		else if (GetDlgItem(Object_RotY) == GetFocus())
		{
			m_vecRotaion.y += m_fMovePower;
		}
		else if (GetDlgItem(Object_RotZ) == GetFocus())
		{
			m_vecRotaion.z += m_fMovePower;
		}
		

		m_pNowObject->getTransform()->setScale(m_vecScale);
		//m_pNowObject->getTransform()->setToolAngle(m_vecRotaion);
		m_pNowObject->getTransform()->setAngle(m_vecRotaion);
		m_pNowObject->getTransform()->setPos(m_vecPosition);



		UpdateData(FALSE);
	}
	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}


void CForm::OnTvnSelchangedObject(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	m_TreeNow = pNMTreeView->itemNew.hItem;

	LinkResourceAndVariableQuad();

	*pResult = 0;
}


void CForm::OnNMSetfocusObject(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here

	LinkResourceAndVariableQuad();

	*pResult = 0;
}


void CForm::OnLbnKillfocusTerrain()
{
	// TODO: Add your control notification handler code here
	m_List_Terrain.SetCurSel(-1);
}


void CForm::OnBnClickedCloneObject()
{
	// TODO: Add your control notification handler code here
}


void CForm::OnBnClickedObjectSave()
{
	// TODO: Add your control notification handler code here
		//다이얼로그열기
	std::string section;
	std::string Key;
	std::string Value;

	section = "QuadCount";

	Key = "Count";

	Value = to_string(m_pMapToolView->m_listQuad.size());

	m_pIniManager->AddData(section, Key, Value);

	int i = 0;

	for (const auto& Quad : m_pMapToolView->m_listQuad)
	{
		section = string_format("Quad_%d", i++);
		/*Key = "Info";

		VTXINFO* NowTerrainInfo = nullptr;
		static_cast<CToolGameObject*>(Quad)->Get_VTXINFO(&NowTerrainInfo);
		Value = string_format("%d,%d,%d,%d", NowTerrainInfo->X, NowTerrainInfo->Z, NowTerrainInfo->Interval, NowTerrainInfo->Detail);

		m_pIniManager->AddData(section, Key, Value);*/

		Key = "FolderFileName";

		const std::vector<TEXTUREINFO>& vecTextureInfo = static_cast<CToolGameObject*>(Quad)->Get_vecTextureInfo();
		for (const auto& TextureInfo : vecTextureInfo)
		{
			string folderName{ TextureInfo.strTextureFolder.begin(),TextureInfo.strTextureFolder.end() };
			string FileName{ TextureInfo.strTextureName.begin(),TextureInfo.strTextureName.end() };
			Value = folderName + "," + FileName;

			m_pIniManager->AddData(section, Key, Value);
		}


		Key = "ObjectAndTypeName";
		CString strObjectName;
		static_cast<CToolGameObject*>(Quad)->Get_ObjectName(strObjectName);
		string ObjectName = std::string(CT2CA(strObjectName));

		CString strTypeName;
		static_cast<CToolGameObject*>(Quad)->Get_TypeName(strTypeName);
		string TypeName = std::string(CT2CA(strTypeName));

		Value = ObjectName + "," + TypeName;

		m_pIniManager->AddData(section, Key, Value);

		Key = "Scale";
		const _vec3& vec3Scale = Quad->getTransform()->getScale();
		Value = string_format("%f,%f,%f", vec3Scale.x, vec3Scale.y, vec3Scale.z);
		m_pIniManager->AddData(section, Key, Value);


		Key = "Euler Angle";
		const _vec3& pVec = Quad->getTransform()->getToolAngle();
		Value = string_format("%f,%f,%f", pVec.x, pVec.y, pVec.z);
		m_pIniManager->AddData(section, Key, Value);

		Key = "Position";
		const _vec3& vec3Position = Quad->getTransform()->getPos();
		Value = string_format("%f,%f,%f", vec3Position.x, vec3Position.y, vec3Position.z);
		m_pIniManager->AddData(section, Key, Value);


	}

	m_pIniManager->SaveIni(std::string("QuadData"));

}

void CForm::OnBnClickedObjectLoad()
{
	// TODO: Add your control notification handler code here
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int QuadSize = m_pIniManager->LoadDataInteger(std::string("QuadData"), "QuadCount", "Count");
	if (!m_pMapToolView->m_listQuad.empty() )
	{
		std::for_each(m_pMapToolView->m_listQuad.begin(), m_pMapToolView->m_listQuad.end(), DeleteObj);
		m_pMapToolView->m_listQuad.clear();
		/*std::for_each(m_pMapToolView->m_listCube.begin(), m_pMapToolView->m_listCube.end(), DeleteObj);
		m_pMapToolView->m_listCube.clear();
		m_Tree_Object.DeleteAllItems();
		m_TreeRoot = m_Tree_Object.InsertItem(L"Object", 0, 0, TVI_ROOT, TVI_LAST);*/
	}

	std::string Section;
	std::string Key;
	std::string Value;
	CGameObject* pQuad = nullptr;

	for (int i = 0; i < QuadSize; ++i)
	{
		Section = string_format("Quad_%d", i);
		size_t dot = 0;
		int PointerSize = 0;
		/*Key = "Info";

		std::string VtxInfoValue = m_pIniManager->LoadDataString(std::string("QuadData"), Section, Key);

		VTXINFO NowTerrainInfo;

		
		while (true)
		{
			if (VtxInfoValue.find(',') == std::string::npos)
			{
				Value = VtxInfoValue.substr(0, VtxInfoValue.size());
				*(((int*)&NowTerrainInfo) + (PointerSize)) = stoi(Value);
				PointerSize = 0;
				break;
			}
			dot = VtxInfoValue.find(',');
			Value = VtxInfoValue.substr(0, dot);
			*(((int*)&NowTerrainInfo) + (PointerSize++)) = stoi(Value);

			VtxInfoValue.erase(0, dot + 1);
		}*/

		pQuad = CQuadObject::Create(m_pMapToolView->m_pDevice);

		std::wstring FolderName;
		std::wstring FileName;

		Key = "FolderFileName";


		std::string ObjectAndTypeName = m_pIniManager->LoadDataString(std::string("QuadData"), Section, Key);

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

		dynamic_cast<CToolGameObject*>(pQuad)->Set_Path(FolderName, FileName, 0);

		TCHAR strtemp[MAX_PATH] = L"..\\..\\Client\\Bin\\Resource\\Texture\\";
		lstrcat(strtemp, FolderName.c_str());
		lstrcat(strtemp, L"\\");
		lstrcat(strtemp, FileName.c_str());
		lstrcat(strtemp, L".png");
		if (!GetTexture(FileName.c_str(), TEXTURETYPE::TEX_NORMAL))
			Insert_Texture(m_pMapToolView->m_pGraphicDev->getDevice(), TEXTURETYPE::TEX_NORMAL, strtemp, FileName.c_str(), 1);

		std::vector<TEXTUREINFO> vecTextureInfo = static_cast<CToolGameObject*>(pQuad)->Get_vecTextureInfo();

		vecTextureInfo[0].pTexture = CTexture::Create(m_pMapToolView->m_pDevice);
		vecTextureInfo[0].pTexture->setTexture(GetTexture(FileName.c_str(), TEXTURETYPE::TEX_NORMAL));

		static_cast<CToolGameObject*>(pQuad)->Set_vecTextureInfo(vecTextureInfo);

		std::wstring ObjectName;
		std::wstring TypeName;

		Key = "ObjectAndTypeName";

		std::string FileFolderName = m_pIniManager->LoadDataString(std::string("QuadData"), Section, Key);

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

		dynamic_cast<CToolGameObject*>(pQuad)->Set_ObjectName(CString(ObjectName.c_str()));
		dynamic_cast<CToolGameObject*>(pQuad)->Set_TypeName(CString(TypeName.c_str()));


		Key = "Scale";

		std::string strScale = m_pIniManager->LoadDataString(std::string("QuadData"), Section, Key);

		_vec3 Scale{};

		while (true)
		{
			if (strScale.find(',') == std::string::npos)
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

		std::string strAngle = m_pIniManager->LoadDataString(std::string("QuadData"), Section, Key);

		_vec3 Roatate{};
		while (true)
		{
			if (strAngle.find(',') == std::string::npos)
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

		std::string strPos = m_pIniManager->LoadDataString(std::string("QuadData"), Section, Key);

		_vec3 Position{};
		while (true)
		{
			if (strPos.find(',') == std::string::npos)
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

		pQuad->getTransform()->setScale(Scale);
		pQuad->getTransform()->setAngle(Roatate);
		pQuad->getTransform()->setPos(Position);

		m_pMapToolView->m_listQuad.emplace_back(pQuad);

		UpdateData(TRUE);
	}
}


void CForm::OnBnClickedCubesave()
{
	// TODO: Add your control notification handler code here
	std::string section;
	std::string Key;
	std::string Value;

	section = "CubeCount";

	Key = "Count";

	Value = to_string(m_pMapToolView->m_listCube.size());

	m_pIniManager->AddData(section, Key, Value);

	int i = 0;

	for (const auto& Cube : m_pMapToolView->m_listCube)
	{
		section = string_format("Cube_%d", i++);
		/*Key = "Info";

		VTXINFO* NowTerrainInfo = nullptr;
		static_cast<CToolGameObject*>(Cube)->Get_VTXINFO(&NowTerrainInfo);
		Value = string_format("%d,%d,%d,%d", NowTerrainInfo->X, NowTerrainInfo->Z, NowTerrainInfo->Interval, NowTerrainInfo->Detail);

		m_pIniManager->AddData(section, Key, Value);*/

		Key = "FolderFileName";

		const std::vector<TEXTUREINFO>& vecTextureInfo = static_cast<CToolGameObject*>(Cube)->Get_vecTextureInfo();
		std::string AllFolderFile;
		Value.clear();
		for (const auto& TextureInfo : vecTextureInfo)
		{
			if (Value.empty())
			{
				Value.assign(TextureInfo.strTextureFolder.begin(), TextureInfo.strTextureFolder.end());
			}
			else
			{
				Value += ',' + std::string(TextureInfo.strTextureFolder.begin(), TextureInfo.strTextureFolder.end());
			}
			Value += ',';
			Value += std::string(TextureInfo.strTextureName.begin(), TextureInfo.strTextureName.end());
			
		}
		m_pIniManager->AddData(section, Key, Value);


		Key = "ObjectAndTypeName";
		CString strObjectName;
		static_cast<CToolGameObject*>(Cube)->Get_ObjectName(strObjectName);
		string ObjectName = std::string(CT2CA(strObjectName));

		CString strTypeName;
		static_cast<CToolGameObject*>(Cube)->Get_TypeName(strTypeName);
		string TypeName = std::string(CT2CA(strTypeName));

		Value = ObjectName + "," + TypeName;

		m_pIniManager->AddData(section, Key, Value);

		Key = "Scale";
		const _vec3& vec3Scale = Cube->getTransform()->getScale();
		Value = string_format("%f,%f,%f", vec3Scale.x, vec3Scale.y, vec3Scale.z);
		m_pIniManager->AddData(section, Key, Value);

		Key = "Euler Angle";
		const _vec3& pVec = Cube->getTransform()->getToolAngle();
		Value = string_format("%f,%f,%f", pVec.x, pVec.y, pVec.z);
		m_pIniManager->AddData(section, Key, Value);

		Key = "Position";
		const _vec3& vec3Position = Cube->getTransform()->getPos();
		Value = string_format("%f,%f,%f", vec3Position.x, vec3Position.y, vec3Position.z);
		m_pIniManager->AddData(section, Key, Value);


	}

	m_pIniManager->SaveIni(std::string("CubeData"));
}


void CForm::OnBnClickedCubeLoad()
{
	// TODO: Add your control notification handler code here
	int QuadSize = m_pIniManager->LoadDataInteger(std::string("CubeData"), "CubeCount", "Count");

	//이것은 수정이 필요함
	/*if (!m_pMapToolView->m_listQuad.empty() || !m_pMapToolView->m_listCube.empty())
	{
		std::for_each(m_pMapToolView->m_listQuad.begin(), m_pMapToolView->m_listQuad.end(), DeleteObj);
		m_pMapToolView->m_listQuad.clear();
		std::for_each(m_pMapToolView->m_listCube.begin(), m_pMapToolView->m_listCube.end(), DeleteObj);
		m_pMapToolView->m_listCube.clear();
		m_Tree_Object.DeleteAllItems();
		m_TreeRoot = m_Tree_Object.InsertItem(L"Object", 0, 0, TVI_ROOT, TVI_LAST);
	}*/

	std::string Section;
	std::string Key;
	std::string Value;
	CGameObject* pCube = nullptr;

	for (int i = 0; i < QuadSize; ++i)
	{
		Section = string_format("Cube_%d", i);
		size_t dot = 0;
		int PointerSize = 0;
	/*	Key = "Info";

		std::string VtxInfoValue = m_pIniManager->LoadDataString(std::string("CubeData"), Section, Key);

		VTXINFO NowTerrainInfo;

		while (true)
		{
			if (VtxInfoValue.find(',') == std::string::npos)
			{
				Value = VtxInfoValue.substr(0, VtxInfoValue.size());
				*(((int*)&NowTerrainInfo) + (PointerSize)) = stoi(Value);
				PointerSize = 0;
				break;
			}
			dot = VtxInfoValue.find(',');
			Value = VtxInfoValue.substr(0, dot);
			*(((int*)&NowTerrainInfo) + (PointerSize++)) = stoi(Value);

			VtxInfoValue.erase(0, dot + 1);
		}*/

		pCube = CCubeObject::Create(m_pMapToolView->m_pDevice);

		std::wstring FolderName;
		std::wstring FileName;

		Key = "FolderFileName";

		std::string ObjectAndTypeName = m_pIniManager->LoadDataString(std::string("CubeData"), Section, Key);

		std::vector<TEXTUREINFO> vecTextureInfo = static_cast<CToolGameObject*>(pCube)->Get_vecTextureInfo();
		while (true)
		{
			
			dot = ObjectAndTypeName.find(',');
			Value = ObjectAndTypeName.substr(0, dot);
			FolderName.assign(Value.begin(), Value.end());
			ObjectAndTypeName.erase(0, dot + 1);

			if (ObjectAndTypeName.find(',') == std::string::npos)
			{
				Value = ObjectAndTypeName.substr(0, ObjectAndTypeName.size());
				FileName.assign(Value.begin(), Value.end());

				dynamic_cast<CToolGameObject*>(pCube)->Set_Path(FolderName, FileName, PointerSize);

				TCHAR strtemp[MAX_PATH] = L"..\\..\\Client\\Bin\\Resource\\Texture\\";
				lstrcat(strtemp, FolderName.c_str());
				lstrcat(strtemp, L"\\");
				lstrcat(strtemp, FileName.c_str());
				lstrcat(strtemp, L".png");
				if (!GetTexture(FileName.c_str(), TEXTURETYPE::TEX_NORMAL))
					Insert_Texture(m_pMapToolView->m_pGraphicDev->getDevice(), TEXTURETYPE::TEX_NORMAL, strtemp, FileName.c_str(), 1);


				vecTextureInfo[PointerSize].pTexture = CTexture::Create(m_pMapToolView->m_pDevice);
				vecTextureInfo[PointerSize].pTexture->setTexture(GetTexture(FileName.c_str(), TEXTURETYPE::TEX_NORMAL));
				PointerSize = 0;
				break;
			}

			dot = ObjectAndTypeName.find(',');
			Value = ObjectAndTypeName.substr(0, dot);
			FileName.assign(Value.begin(), Value.end());
			ObjectAndTypeName.erase(0, dot + 1);
		
			dynamic_cast<CToolGameObject*>(pCube)->Set_Path(FolderName, FileName, PointerSize);

			TCHAR strtemp[MAX_PATH] = L"..\\..\\Client\\Bin\\Resource\\Texture\\";
			lstrcat(strtemp, FolderName.c_str());
			lstrcat(strtemp, L"\\");
			lstrcat(strtemp, FileName.c_str());
			lstrcat(strtemp, L".png");
			if (!GetTexture(FileName.c_str(), TEXTURETYPE::TEX_NORMAL))
				Insert_Texture(m_pMapToolView->m_pGraphicDev->getDevice(), TEXTURETYPE::TEX_NORMAL, strtemp, FileName.c_str(), 1);

			vecTextureInfo[PointerSize].pTexture = CTexture::Create(m_pMapToolView->m_pDevice);
			vecTextureInfo[PointerSize].pTexture->setTexture(GetTexture(FileName.c_str(), TEXTURETYPE::TEX_NORMAL));
			++PointerSize;
		}


		static_cast<CToolGameObject*>(pCube)->Set_vecTextureInfo(vecTextureInfo);

		std::wstring ObjectName;
		std::wstring TypeName;

		Key = "ObjectAndTypeName";

		std::string FileFolderName = m_pIniManager->LoadDataString(std::string("CubeData"), Section, Key);

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

		dynamic_cast<CToolGameObject*>(pCube)->Set_ObjectName(CString(ObjectName.c_str()));
		dynamic_cast<CToolGameObject*>(pCube)->Set_TypeName(CString(TypeName.c_str()));


		Key = "Scale";

		std::string strScale = m_pIniManager->LoadDataString(std::string("CubeData"), Section, Key);

		_vec3 Scale{};

		while (true)
		{
			if (strScale.find(',') == std::string::npos)
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

		std::string strAngle = m_pIniManager->LoadDataString(std::string("CubeData"), Section, Key);

		_vec3 Roatate{};
		while (true)
		{
			if (strAngle.find(',') == std::string::npos)
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

		std::string strPos = m_pIniManager->LoadDataString(std::string("CubeData"), Section, Key);

		_vec3 Position{};
		while (true)
		{
			if (strPos.find(',') == std::string::npos)
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

		pCube->getTransform()->setScale(Scale);
		pCube->getTransform()->setAngle(Roatate);
		pCube->getTransform()->setPos(Position);

		m_pMapToolView->m_listCube.emplace_back(pCube);

		UpdateData(TRUE);
	}
}
